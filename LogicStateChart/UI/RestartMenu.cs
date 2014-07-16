//#define windows

using System;
using System.Runtime.CompilerServices;
using ScriptRuntime;
using ScriptGUI;
using System.Diagnostics;
using Logic;

namespace UserDefGUI
{
    public class RestartMenu
    {
        private FString m_windowName = "RestartMenu";

        public void Init()
        {
            GUI.RegisterLayout(m_windowName, "Layout/RestartMenu.layout", false, false);
            GUI.UIWidget.SetEventMouseButtonClick(m_windowName, "RestartBtn", OnRestartBtnClick, EventControl.Add);
        }

        private void OnRestartBtnClick(FString sender)
        {
            GUI.SetLayoutVisible(m_windowName, false);
            Logic.SceneMgr.Instance.player.Reset();
        }

        public void SetVisable(bool vis)
        {
            GUI.SetLayoutVisible(m_windowName, vis);
        }

    };
}