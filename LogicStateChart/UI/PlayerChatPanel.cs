using System;
using System.Runtime.CompilerServices;
using ScriptRuntime;
using ScriptGUI;
using RPGData;
using Logic;

namespace UserDefGUI
{
    public class PlayerChatPanel
    {
        private FString m_windowName = "PlayerChatPanel";
        private FString m_NpcOldmanName = "NPC_Oldman";
        private FString m_NpcDaChuiName = "NPC_DaChui";
        private FString m_textBoxName = "chatBox";
        //private string m_currentText = "";

        public void Init()
        {
            GUI.RegisterLayout(m_windowName, "Layout/DialogueBG.layout", false, false);
            SetAllNpcVis(false);
        }

        public void SetNpcOldManVis(bool vis)
        {
            GUI.UIWidget.SetVisible(m_windowName, m_NpcOldmanName, vis);
        }

        public void SetNpcDaChuiManVis(bool vis)
        {
            GUI.UIWidget.SetVisible(m_windowName, m_NpcDaChuiName, vis);
        }

        public void SetAllNpcVis(bool vis)
        {
            SetNpcOldManVis(vis);
            SetNpcDaChuiManVis(vis);
        }

        public void SetNPCVis(string name)
        {
            switch (name)
            {
                case "NPC_tiejiang":
                    SetNpcDaChuiManVis(true);
                    break;
                case "NPC_xinshouzhiyin":
                    SetNpcOldManVis(true);
                    break;
            }
        }

        public void SetLayoutVis(bool vis)
        {
            GUI.SetLayoutVisible(m_windowName, vis);
        }

        public void SetChatCaption(string chatContent)
        {
            GUI.UITextBox.SetCaption(m_windowName, m_textBoxName, chatContent);
        }

    };
}