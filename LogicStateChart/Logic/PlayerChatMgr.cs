using System;
using System.Runtime.CompilerServices;
using ScriptRuntime;
using RPGData;

namespace Logic
{
    public class PlayerChatMgr : Singleton<PlayerChatMgr>
    {

        public void Init()
        {
            CurrentChatNPCName = string.Empty;
            CurrentChatContentIndex = 0;
        }

        private string CurrentChatNPCName
        {
            get
            {
                return m_sCurrentChatNPCName;
            }
            set
            {
                m_sCurrentChatNPCName = value;
            }
        }

        private int CurrentChatContentIndex
        {
            get
            {
                return m_iCurrentChatContentIndex;
            }
            set
            {
                m_iCurrentChatContentIndex = value;
            }
        }

        //返回当前对话内容，并自增对话索引
        public NPCChat PlayerChatToNPC(string sNPCName)
        {
            NPCChat chat = null;
            if (NPCMgr.Instance.CheckChatArea(sNPCName))
            {
                if (string.IsNullOrEmpty(CurrentChatNPCName) || !CurrentChatNPCName.Equals(sNPCName))
                {
                    CurrentChatNPCName = sNPCName;
                    CurrentChatContentIndex = 0;
                }

                chat = AllNPCChatConfig.Instance.GetNPCChat(CurrentChatNPCName, CurrentChatContentIndex);
                if (null == chat)
                {
                    CurrentChatNPCName = string.Empty;
                    CurrentChatContentIndex = 0;
                }
                else
                {
                    ++CurrentChatContentIndex;
                }
            }
            return chat;
        }

        private string m_sCurrentChatNPCName;
        private int m_iCurrentChatContentIndex;

    }
}