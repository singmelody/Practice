using System;
using System.Runtime.CompilerServices;
using ScriptRuntime;
using ScriptGUI;
using RPGData;
using Logic;

namespace UserDefGUI
{
    public class PlayerChatPanelMgr : Singleton<PlayerChatPanelMgr>
    {
        private PlayerChatPanel m_playerChatPanel = new PlayerChatPanel();

        public void Init()
        {
            m_playerChatPanel.Init();
        }

        public void ResetPanel(NPCChat chat, string NPCName)
        {
            if (chat == null)
            {
                m_playerChatPanel.SetLayoutVis(false);
            }
            else
            {
                m_playerChatPanel.SetChatCaption(chat.ChatContent);
                if (chat.SpeakerID == NPCChatSpeakerID.Player)
                {
                    m_playerChatPanel.SetAllNpcVis(false);
                }
                else
                {
                    m_playerChatPanel.SetNPCVis(NPCName);
                }
                m_playerChatPanel.SetLayoutVis(true);
            }
        }

        public void ClosePanel()
        {
            m_playerChatPanel.SetLayoutVis(false);
        }

    };
}