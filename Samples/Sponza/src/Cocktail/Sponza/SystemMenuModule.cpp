#include <Cocktail/Core/Color.hpp>
#include <Cocktail/Core/Log/LogManager.hpp>
#include <Cocktail/Core/Utility/EnumMap.hpp>

#include <Cocktail/Sponza/ImUi/ImUi.hpp>
#include <Cocktail/Sponza/SystemMenuModule.hpp>

namespace Ck
{
    class LogPanel : public ImUi::Panel
    {
    public:

        LogPanel(LogManager* logManager) :
            mDirty(true),
            mAutoScroll(true),
            mLogManager(logManager),
            mCurrentCategory(nullptr)
        {
            mColors[LogLevel::Debug] = LinearColor::White;
            mDisplayedLevels[LogLevel::Debug] = false;

            mColors[LogLevel::Info] = LinearColor::Blue;
            mDisplayedLevels[LogLevel::Info] = true;

            mColors[LogLevel::Warning] = LinearColor::Yellow;
            mDisplayedLevels[LogLevel::Warning] = true;

            mColors[LogLevel::Error] = LinearColor::Red;
            mDisplayedLevels[LogLevel::Error] = true;

            mColors[LogLevel::Critical] = LinearColor::Red;
            mDisplayedLevels[LogLevel::Critical] = true;
        }

        void Attach() override
        {
            Panel::Attach();

            PushLogCategory(nullptr);
            for (LogEntry* entry : mLogManager->GetEntries())
                PushLogCategory(entry->Category);

            Connect(mLogManager->OnTraceEntry(), [&](LogEntry* entry) {
                PushLogCategory(entry->Category);
                if (mCurrentCategory == nullptr || entry->Category == mCurrentCategory)
                    mEntries.Add(entry);
            });
        }

        void Update(const Duration& deltaTime) override
        {
            Panel::Update(deltaTime);

            if (mDirty)
            {
                mEntries.Clear();
                for (LogEntry* entry : mLogManager->GetEntries())
                {
                    if (mCurrentCategory && entry->Category != mCurrentCategory)
                        continue;

                    if (!mDisplayedLevels[entry->Level])
                        continue;

                    mEntries.Add(entry);
                }

                mDirty = false;
            }
        }

        void Render() override
        {
            if (ImGui::Begin("Log Viewer", &mOpen))
            {
                ImGui::SetNextItemWidth(180.0f);
                if (ImGui::BeginCombo("Log category", mCurrentCategory ? CK_TEXT_TO_IMGUI(mCurrentCategory->GetName()) : nullptr))
                {
                    for (const LogCategory* logCategory : mLogCategories)
                    {
                        const bool selected = logCategory == mCurrentCategory;
                        if (ImGui::Selectable(logCategory ? CK_TEXT_TO_IMGUI(logCategory->GetName()) : "##hidden", selected))
                            mDirty = CheckedAssign(mCurrentCategory, logCategory);

                        if (selected)
                            ImGui::SetItemDefaultFocus();
                    }

                    ImGui::EndCombo();
                }

                ImGui::SameLine();

                ImGui::SetNextItemWidth(50.0f);
                ImGui::Checkbox("Auto-scroll", &mAutoScroll);

                ImGui::SameLine();

                ImGui::SetNextItemWidth(180.0f);
                if (ImGui::BeginCombo("Log level", "Choose..."))
                {
                    for (LogLevel logLevel : Enum<LogLevel>::Values)
                    {
                        bool& selected = mDisplayedLevels[logLevel];
                        const AnsiChar* label = Enum<LogLevel>::ToString(logLevel);
                        if (ImGui::Selectable(label, &selected, ImGuiSelectableFlags_DontClosePopups))
                            mDirty = true;
                    }
                    ImGui::EndCombo();
                }

                ImGui::Separator();

                if (ImGui::BeginChild("scrolling", ImVec2(0, 0), ImGuiChildFlags_None, ImGuiWindowFlags_HorizontalScrollbar))
                {
                    ImGuiListClipper clipper;
                    clipper.Begin(mEntries.GetSize());
                    while (clipper.Step())
                    {
                        for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
                        {
                            LogEntry* logEntry = mEntries[i];
                            ImGui::TextColored(GetLevelColor(logEntry->Level), "[%s] %s", CK_TEXT_TO_IMGUI(logEntry->Category->GetName()), CK_TEXT_TO_IMGUI(logEntry->Message));
                        }
                    }
                    clipper.End();
                }

                if (mAutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
                    ImGui::SetScrollHereY(1.0f);

                ImGui::EndChild();
            }
            ImGui::End();
        }

        const AnsiChar* GetName() const override
        {
            return "LogViewer";
        }

    private:

        void PushLogCategory(const LogCategory* category)
        {
            if (!mLogCategories.Contains(category))
                mLogCategories.Add(category);
        }

        ImVec4 GetLevelColor(LogLevel level)
        {
            const LinearColor& color = mColors[level];
            return ImVec4(color.R, color.G, color.B, color.A);
        }

        bool mDirty;
        bool mAutoScroll;
        LogManager* mLogManager;
        Array<LogEntry*> mEntries;
        const LogCategory* mCurrentCategory;
        Array<const LogCategory*> mLogCategories;
        EnumMap<LogLevel, LinearColor> mColors;
        EnumMap<LogLevel, bool> mDisplayedLevels;
    };

    void SystemMenuModule::Attach()
    {
        CreatePanel([&](LogManager* logManager) {
            return GetPanelManager()->CreatePanel<LogPanel>(logManager);
        });
    }

    const AnsiChar* SystemMenuModule::GetName() const
    {
        return "System";
    }
}
