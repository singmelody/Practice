using System;
using System.Runtime.CompilerServices;
using ScriptRuntime;
using System.Collections.Generic;
using System.IO;

namespace RPGData
{
	public class EnemyAnimationConfig : Logic.Singleton<EnemyAnimationConfig>
	{
		
		public EnemyAnimationConfig()
		{
			m_vEnemyAnimation = new Dictionary<string, string>();
			LoadEnemyAnimation();
		}
		
		private string GenerateEnemyAnimationTablePath()
		{
			string sAssetPath = ScriptRuntime.Application.GetAssetPath();
			string sPath = sAssetPath + ENEMYANIMATION_PATH;
			return sPath;
		}
		
		private void LoadEnemyAnimation()
		{
			StreamReader streamRead = new StreamReader(GenerateEnemyAnimationTablePath(), System.Text.Encoding.Default);
			string sLine = streamRead.ReadLine();
			string[] aryLine;
			while (null != (sLine = streamRead.ReadLine()))
			{
				aryLine = sLine.Split(new char[] { '\t' });
				if (COLUMN_PER_NPCANIMATION > aryLine.Length) continue;
				
				string sEnemyName = aryLine[0];
				string sAnimationPath = aryLine[1];
				EnemyAnimationDictionary.Add(sEnemyName, sAnimationPath);
			}
			streamRead.Close();
		}
		
		public void Init() { }
		
		public Dictionary<string, string> EnemyAnimationDictionary
		{
			get
			{
				return m_vEnemyAnimation;
			}
			set
			{
				m_vEnemyAnimation = value;
			}
		}
		
		public string GetEnemyAnimationPath(string sEnemyName)
		{
			if (EnemyAnimationDictionary.ContainsKey(sEnemyName))
			{
				return EnemyAnimationDictionary[sEnemyName];
			}
			else
			{
				return string.Empty;
			}
		}
		
		private const string ENEMYANIMATION_PATH = "/Script/DataTable/EnemyAnimation.csv";
		private const Int32 COLUMN_PER_NPCANIMATION = 2;
		
		private Dictionary<string, string> m_vEnemyAnimation;         //<怪物名字，怪物默认动画路径>
	}
}