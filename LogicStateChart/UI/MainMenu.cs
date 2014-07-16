//#define windows

using System;
using System.Runtime.CompilerServices;
using ScriptRuntime;
using ScriptGUI;
using System.Diagnostics;
using Logic;

namespace UserDefGUI
{
    public class MainMenu
    {
        private FString m_windowName = "MainMenu";

        public void Init()
        {
            GUI.RegisterLayout(m_windowName, "Layout/MainMenu.layout", false, true);
            GUI.UIWidget.SetEventMouseButtonClick(m_windowName, "StartBtn", OnStartBtnClick, EventControl.Add);
            GUI.UIWidget.SetEventMouseButtonClick(m_windowName, "QuitBtn", OnQuitBtnClick, EventControl.Add);
        }

        private void OnStartBtnClick(FString sender)
        {
            GUI.SetLayoutVisible(m_windowName, false);
            SceneMgr.Instance.GamePause = false;
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