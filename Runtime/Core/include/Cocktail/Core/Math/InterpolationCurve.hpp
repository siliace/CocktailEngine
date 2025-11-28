#ifndef COCKTAIL_CORE_MATH_INTERPOLATIONCURVE_HPP
#define COCKTAIL_CORE_MATH_INTERPOLATIONCURVE_HPP

#include <map>

#include <Cocktail/Core/Array.hpp>
#include <Cocktail/Core/Math/Interpolation.hpp>
#include <Cocktail/Core/Math/Vector/Vector2.hpp>
#include <Cocktail/Core/Math/Vector/Vector3.hpp>
#include <Cocktail/Core/Math/Vector/Vector4.hpp>

namespace Ck
{
    /**
     * \brief Class implementing a generic interpolation curve defined by key–value pairs
     *
     * This class stores a set of ordered keyframes and computes interpolated values
     * between them using a user-provided interpolation function. Keys typically
     * represent time or any monotonic parameter, while values represent the data
     * being interpolated (e.g., positions, scales, floats).
     *
     * \tparam TKey The type used to identify keyframes (must support comparison and arithmetic operations).
     * \tparam TValue The value type of each keyframe. Must be an arithmetic type.
     */
    template <typename TKey, typename TValue>
    class InterpolationCurve
    {
    public:

        using KeyType = TKey;
        using ValueType = TValue;

        static_assert(std::is_arithmetic_v<ValueType>, "Interpolation key type should be arithmetic");

        /**
         * \brief Defines an interpolation function
         */
        using InterpolationFunction = ValueType(*)(const ValueType&, const ValueType&, float);

        /**
         * \brief Represents a single keyframe of the interpolation curve
         *
         * A keyframe associates a key (typically time) to a value to be interpolated
         */
        struct KeyFrame
        {
            KeyType Key;
            ValueType Value;
        };

        /**
         * \brief Helper class used to construct an InterpolationCurve
         *
         * The Builder allows setting keyframes in any order and specifying a custom
         * interpolation function. Keyframes are internally sorted by key.
         */
        class Builder
        {
        public:

            /**
             * \brief Default constructor
             *
             * Creates a new instance of Builder with a default linear interpolation function.
             */
            Builder() :
                mInterpolationFunction(Lerp)
            {
                /// Nothing
            }

            /**
             * \brief Sets the interpolation function used by the curve
             *
             * \param interpolationFunction The function that will be used by the curve
             *
             * \return Reference to the Builder for chaining
             */
            Builder& SetInterpolationFunction(InterpolationFunction interpolationFunction)
            {
                mInterpolationFunction = interpolationFunction;
                return *this;
            }

            /**
             * \brief Adds or updates a keyframe
             *
             * If a keyframe with the same key already exists, its value is replaced
             *
             * \param key The key of the frame
             * \param value The value associated with the key
             *
             * \return Reference to the Builder for chaining
             */
            Builder& SetKeyFrame(const KeyType& key, const ValueType& value)
            {
                mKeyFrames.insert_or_assign(key, value);
                return *this;
            }

            /**
             * \brief Builds the final InterpolationCurve instance
             *
             * Keyframes are automatically sorted by key before being stored
             *
             * \return A fully constructed InterpolationCurve
             */
            InterpolationCurve Get()
            {
                Array<KeyFrame> keyFrames;
                keyFrames.Reserve(mKeyFrames.size());

                for (const auto& [key, value] : mKeyFrames)
                    keyFrames.Add(KeyFrame{ key, value });

                return InterpolationCurve(std::move(keyFrames), mInterpolationFunction);
            }

        private:

            std::map<KeyType, ValueType> mKeyFrames;
            InterpolationFunction mInterpolationFunction;
        };

        /**
         * \brief Constructor
         *
         * Creates a new empty instance of interpolation curve using the given interpolation function.
         *
         * \param interpolationFunction The function used when interpolating between keyframes
         */
        InterpolationCurve(InterpolationFunction interpolationFunction = Lerp) :
            mInterpolationFunction(interpolationFunction)
        {
            /// Nothing
        }

        /**
         * \brief Adds or updates a keyframe in the curve
         *
         * If a keyframe with the same key already exists, it is replaced.
         * Otherwise, the keyframe is inserted at the correct position to maintain order.
         *
         * \param key The key of the frame
         * \param value The value associated with the key
         */
        void AddKeyFrame(const KeyType& key, const ValueType& value)
        {
            if (!mKeyFrames.IsEmpty())
            {
                unsigned int index = mKeyFrames.FindIndexIf([&](const KeyFrame& keyFrame) {
                    return keyFrame.Key < key;
                }).GetOr(0);

                KeyFrame keyFrame{ key, value };
                if (mKeyFrames[index].Key == key)
                {
                    mKeyFrames[index] = std::move(keyFrame);
                }
                else
                {
                    mKeyFrames.AddAt(index, std::move(keyFrame));
                }
            }
            else
            {
                mKeyFrames.Emplace(key, value);
            }
        }

        /**
         * \brief Removes a keyframe associated with the specified key
         *
         * If no keyframe matches the key, nothing happens.
         *
         * \param key The key of the frame to remove
         */
        void RemoveKeyFrame(const KeyType& key)
        {
            mKeyFrames.FilterInPlace([&](const KeyFrame& keyFrame) {
                return key == keyFrame.Key;
            });
        }

        /**
         * \brief Gets an interpolated value for the given key
         *
         * If the key is below the first keyframe, returns the first value.
         * If the key is above the last keyframe, returns the last value.
         * Otherwise, the appropriate two keyframes are found and the interpolation
         * function is applied.
         *
         * \param key The key at which to sample the curve
         *
         * \return The interpolated value
         *
         * \throws OutOfRangeException if the key cannot be interpolated
         */
        ValueType Get(const KeyType& key) const
        {
            assert(!mKeyFrames.IsEmpty());

            if (const KeyFrame& firstKeyFrame = mKeyFrames.First(); key <= firstKeyFrame.Key)
                return firstKeyFrame.Value;

            if (const KeyFrame& lastKeyFrame = mKeyFrames.Last(); key >= lastKeyFrame.Key)
                return lastKeyFrame.Value;

            for (unsigned int i = 0; i < mKeyFrames.GetSize(); i++)
            {
                const auto& [t0, v0] = mKeyFrames[i];
                const auto& [t1, v1] = mKeyFrames[i + 1];

                if (!(key >= mKeyFrames[i].Key && key <= mKeyFrames[i + 1].Key))
                    continue;

                auto alpha = (key - t0) / (t1 - t0);
                return mInterpolationFunction(v0, v1, alpha);
            }

            ExceptionUtils::ThrowOutOfRange(static_cast<Uint32>(mKeyFrames.First().Key), static_cast<Uint32>(mKeyFrames.Last().Key));
        }

        /**
         * \brief Returns the internal array of keyframes
         *
         * The returned array preserves the insertion order (sorted by key).
         *
         * \return A reference to the array of KeyFrame objects
         */
        const Array<KeyType>& GetKeyFrames() const
        {
            return mKeyFrames;
        }

    private:

        /**
         * \brief Internal constructor.
         *
         * Creates a new instance of interpolation curve using the given interpolation function.
         * Used by the Builder to create a fully initialized curve with a predefined set of keyframes.
         *
         * \param keyFrames The sorted collection of keyframes
         * \param interpolationFunction The interpolation function used between frames
         */
        InterpolationCurve(Array<KeyFrame> keyFrames, InterpolationFunction interpolationFunction) :
            mKeyFrames(std::move(keyFrames)),
            mInterpolationFunction(interpolationFunction)
        {
            /// Nothing
        }

        Array<KeyFrame> mKeyFrames;
        InterpolationFunction mInterpolationFunction;
    };

    template <typename T>
    using ScalarInterpolationCurve = InterpolationCurve<T, T>;

    template <typename T>
    using Vector2InterpolationCurve = InterpolationCurve<T, Vector2<T>>;

    template <typename T>
    using Vector3InterpolationCurve = InterpolationCurve<T, Vector3<T>>;

    template <typename T>
    using Vector4InterpolationCurve = InterpolationCurve<T, Vector4<T>>;
}

#endif // COCKTAIL_CORE_MATH_INTERPOLATIONCURVE_HPP
