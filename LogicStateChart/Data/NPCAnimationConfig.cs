using System;
using System.Runtime.CompilerServices;
using ScriptRuntime;
using System.Collections.Generic;
using System.IO;

namespace RPGData
{
    public class NPCAnimationConfig : Logic.Singleton<NPCAnimationConfig>
    {

        public NPCAnimationConfig()
        {
            m_vNPCAnimation = new Dictionary<string, string>();
            LoadNPCAnimation();
        }

        private string GenerateNPCAnimationTablePath()
        {
            string sAssetPath = ScriptRuntime.Application.GetAssetPath();
            string sPath = sAssetPath + NPCANIMATION_PATH;
            return sPath;
        }

        private void LoadNPCAnimation()
        {
            StreamReader streamRead = new StreamReader(GenerateNPCAnimationTablePath(), System.Text.Encoding.Default);
            string sLine = streamRead.ReadLine();
            string[] aryLine;
            while (null != (sLine = streamRead.ReadLine()))
            {
                aryLine = sLine.Split(new char[] { '\t' });
                if (COLUMN_PER_NPCANIMATION > aryLine.Length) continue;

                string sNPCName = aryLine[0];
                string sAnimationPath = aryLine[1];
                NPCAnimationDictionary.Add(sNPCName, sAnimationPath);
            }
            streamRead.Close();
        }

        public void Init() { }

        public Dictionary<string, string> NPCAnimationDictionary
        {
            get
            {
                return m_vNPCAnimation;
            }
            set
            {
                m_vNPCAnimation = value;
            }
        }

        public string GetNPCAnimationPath(string sNPCName)
        {
            if (NPCAnimationDictionary.ContainsKey(sNPCName))
            {
                return NPCAnimationDictionary[sNPCName];
            }
            else
            {
                return string.Empty;
            }
        }

        private const string NPCANIMATION_PATH = "/Script/DataTable/NPCAnimation.csv";
        private const Int32 COLUMN_PER_NPCANIMATION = 2;

        private Dictionary<string, string> m_vNPCAnimation;         //<NPC名字，NPC默认动画路径>
    }
}