using System;
using System.Runtime.CompilerServices;
using ScriptRuntime;
using ScriptGUI;
using RPGData;
using Logic;

namespace UserDefGUI
{
    public class LoadingPanel
    {

        private FString m_windowName = "LoadingPanel";

        public void Init()
        {
            GUI.RegisterLayout(m_windowName, "Layout/LoadingPanel.layout", false, false);
        }

        public void SetVisable(bool vis)
        {
            GUI.SetLayoutVisible(m_windowName, vis);
        }

    };
}