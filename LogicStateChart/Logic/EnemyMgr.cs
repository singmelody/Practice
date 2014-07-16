using System;
using System.Runtime.CompilerServices;
using ScriptRuntime;
using System.Collections.Generic;
using RPGData;

namespace Logic
{

	public class EnemyMgr : Singleton<EnemyMgr>
	{
		
		public EnemyMgr()
		{
			m_vEnemy = new List<Enemy>();
		}
		
		private void CreateEnemy(Vector3 pos)
		{
			//temp, also need more information
			Actor actor = new Actor();
			actor.WorldPosition = pos;
			Enemy enemy = new Enemy(actor);
			EnemyList.Add(enemy);
		}

		// return index of enemy in EnemyList, find by using actor 
		int FindEnemyByActor (Actor actor)
		{
			for (int i = 0; i < EnemyList.Count; ++i) 
			{
				 if (EnemyList[i].Data.AvatarActor == actor)
					return i;
			}
			return -1;
		}
		
		public void Init ()
		{
			Clear ();

			for (int i = 0; i < ActorManager.GetActiveActorCount(); ++i) 
			{
				Actor actor = ActorManager.GetActiveActor(i);
				string name = actor.Name;
				if (name.StartsWith(ENEMY_NAMEHEAD))
				{
					Enemy enemy = new Enemy(actor);
					enemy.Init();
					EnemyList.Add (enemy);
				}
			}
		}

		public void Reset ()
		{
			Init ();
		}

		// refresh enemies 
		public void  RestartAll ()
		{
			for (int i = 0; i < EnemyList.Count; i++) 
			{
				Actor actor = EnemyList[i].Data.AvatarActor;
				if (!actor.IsActive) 
				{
					actor.ActiveWithChildren ();
					EnemyList[i].Data.Init();
				}
			}
		}
		
		public void Clear()
		{
			EnemyList.Clear();   
		}

		public void Tick ()
		{
			bool allDie = true;
			for (int i = 0; i < EnemyList.Count; ++i) {
                if (EnemyList[i].Data.AvatarActor.IsActive)
                {
					EnemyList [i].Update ();
					allDie = false;
				}
			}
			if (allDie) 
			{
				RestartAll();
			}
		}

		public List<Enemy> EnemyList
		{
			get
			{
				return m_vEnemy;
			}
		}

		private const string ENEMY_NAMEHEAD = "2_";
		
		private List<Enemy> m_vEnemy;       
	}
	
}
