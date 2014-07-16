using System;
using System.Runtime.CompilerServices;
using ScriptRuntime;
using System.Collections.Generic;
using System.IO;

namespace RPGData
{
    public enum NpcChatName
    {
        NPC_Oldman = 0
    }


    public class NPCChat
    {
        public NPCChat(NPCChatSpeakerID eSpeakerID, string sContent)
        {
            m_eSpeakerID = eSpeakerID;
            m_sContent = sContent;
        }

        public NPCChatSpeakerID SpeakerID
        {
            get
            {
                return m_eSpeakerID;
            }
        }

        public string ChatContent
        {
            get
            {
                return m_sContent;
            }
        }

        private NPCChatSpeakerID m_eSpeakerID;       //说话者
        private string m_sContent;      //对话内容
    }

    public class NPCChatConfig
    {
        public NPCChatConfig(string sNPCName)
        {
            m_sNPCName = sNPCName;
            m_vNPCChat = new Dictionary<int, NPCChat>();
        }

        public string NPCName
        {
            get
            {
                return m_sNPCName;
            }
        }

        public Dictionary<int, NPCChat> NPCChatDictionary
        {
            get
            {
                return m_vNPCChat;
            }
        }

        public void AddNPCChat(NPCChat npcChat)
        {
            int iCount = NPCChatDictionary.Count;
            NPCChatDictionary.Add(iCount, npcChat);
        }

        public NPCChat GetNPCChat(int iIndex)
        {
            if (NPCChatDictionary.ContainsKey(iIndex))
            {
                return NPCChatDictionary[iIndex];
            }
            else
            {
                return null;
            }
        }

        private string m_sNPCName;      //NPC名称
        private Dictionary<int, NPCChat> m_vNPCChat;        //<对话索引，对话内容类>
    }

    public class AllNPCChatConfig : Logic.Singleton<AllNPCChatConfig>
    {
        public AllNPCChatConfig()
        {
            m_vAllNPCChat = new Dictionary<string, NPCChatConfig>();
            foreach (string sNPCName in NPCAnimationConfig.Instance.NPCAnimationDictionary.Keys)
            {
                LoadNPCChat(sNPCName);
            }
        }

        private string GenerateNPCChatPath(string sNPCName)
        {
            string sAssetPath = ScriptRuntime.Application.GetAssetPath();
            string sPath = sAssetPath + NPCCHAT_PATH_PART;
            sPath += sNPCName;
            sPath += NPCCHAT_PATH_END;
            return sPath;
        }

        private void LoadNPCChat(string sNPCName)
        {
            if (File.Exists(GenerateNPCChatPath(sNPCName)))
            {
                if (!AllNPCChatDictionary.ContainsKey(sNPCName))
                {
                    m_vAllNPCChat.Add(sNPCName, new NPCChatConfig(sNPCName));
                }

                StreamReader streamRead = new StreamReader(GenerateNPCChatPath(sNPCName), System.Text.Encoding.Default);
                string sLine = streamRead.ReadLine();
                string[] aryLine;
                while (null != (sLine = streamRead.ReadLine()))
                {
                    aryLine = sLine.Split(new char[] { '\t' });
                    if (COLUMN_PER_CHAT > aryLine.Length) continue;

                    NPCChatSpeakerID eSpeakerID = (NPCChatSpeakerID)int.Parse(aryLine[0]);
                    string sContent = aryLine[1];
                    NPCChat npcChat = new NPCChat(eSpeakerID, sContent);
                    AllNPCChatDictionary[sNPCName].AddNPCChat(npcChat);
                }
                streamRead.Close();
            }
        }

        public void Init() { }

        public Dictionary<string, NPCChatConfig> AllNPCChatDictionary
        {
            get
            {
                return m_vAllNPCChat;
            }
        }

        public NPCChat GetNPCChat(string sNPCName, int iIndex)
        {
            if (AllNPCChatDictionary.ContainsKey(sNPCName))
            {
                return AllNPCChatDictionary[sNPCName].GetNPCChat(iIndex);
            }
            else
            {
                return null;
            }
        }

        private const string NPCCHAT_PATH_PART = "/Script/DataTable/NPCChat/";
        private const string NPCCHAT_PATH_END = ".csv";
        private const Int32 COLUMN_PER_CHAT = 2;

        private Dictionary<string, NPCChatConfig> m_vAllNPCChat;       //<NPC名称，对话类>

    }
}