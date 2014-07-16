//#define windows

using System;
using System.Runtime.CompilerServices;
using ScriptRuntime;
using ScriptGUI;
using Logic;

namespace UserDefGUI
{
    public class ContinueMenu
    {
        private FString m_windowName = "ContinueMenu";
        public void Init()
        {
            GUI.RegisterLayout(m_windowName, "Layout/ContinueMenu.layout", false, true);
            GUI.UIWidget.SetEventMouseButtonClick(m_windowName, "ContinueBtn", OnContinueBtnClick, EventControl.Add);
            GUI.UIWidget.SetEventMouseButtonClick(m_windowName, "QuitBtn", OnQuitBtnClick, EventControl.Add);
            GUI.SetLayoutVisible(m_windowName, false);
        }

        public void SetVisable(bool vis)
        {
            GUI.SetLayoutVisible(m_windowName, vis);
        }

        private void OnContinueBtnClick(FString sender)
        {
            SceneMgr.Instance.GamePause = false;
            UserDefGUIRoot.Instance.SetJsPanelVis(true);
            UserDefGUIRoot.Instance.SetConstUI_CityVis(true);
            GUI.SetLayoutVisible(m_windowName, false);
        }

        private void OnQuitBtnClick(FString sender)
        {
#if windows
            //windows kill process
            System.Diagnostics.Process.GetCurrentProcess().Kill();
#endif
        }

    };
}