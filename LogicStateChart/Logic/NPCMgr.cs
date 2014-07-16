using System;
using System.Runtime.CompilerServices;
using ScriptRuntime;
using System.Collections.Generic;
using RPGData;

namespace Logic
{
    public class NPC
    {
        public NPC(Actor npc)
        {
            Data.AvatarActor = npc;
            AnimationComponent ac = Data.AvatarActor.Animation;
            if (null != ac)
            {
                ac.PlayAnimation(NPCAnimationConfig.Instance.GetNPCAnimationPath(Data.AvatarActor.Name));
            }
        }

        private bool CheckChatArea(Vector3 position)
        {
            int iDistanceX = (int)Math.Abs(Data.AvatarActor.WorldPosition.X - position.X);
            int iDistanceZ = (int)Math.Abs(Data.AvatarActor.WorldPosition.Z - position.Z);
            return (ConstDefine.X_DISTANCE_NPC_CHAT >= iDistanceX && ConstDefine.Z_DISTANCE_NPC_CHAT >= iDistanceZ);
        }

        public bool CheckChatArea()
        {
            return CheckChatArea(SceneMgr.Instance.player.Data.AvatarActor.WorldPosition);
        }

        private AvatarData Data
        {
            get
            {
                return m_NPCData;
            }
            set
            {
                m_NPCData = value;
            }
        }

        private AvatarData m_NPCData;
    }

    public class NPCMgr : Singleton<NPCMgr>
    {

        public NPCMgr()
        {
            m_vNPC = new Dictionary<Actor, NPC>();
        }

        private void Load()
        {
            int iCount = ActorManager.GetActiveActorCount();
            for (int ii = 0; ii < iCount; ++ii)
            {
                Actor npcActor = ActorManager.GetActiveActor(ii);
                if (null == npcActor)
                {
                    throw (new ArgumentException("NPCMgr.Load"));
                }

                if (npcActor.Name.StartsWith(NPC_NAMEHEAD, true, null))
                {
                    NPC npc = new NPC(npcActor);
                    NPCDictionary.Add(npcActor, npc);
                }
            }
        }

        public void Init()
        {
            Load();
        }

        public void Clear()
        {
            NPCDictionary.Clear();
        }

        public void Reset()
        {
            Load();
        }

        public Dictionary<Actor, NPC> NPCDictionary
        {
            get
            {
                return m_vNPC;
            }
        }

        public NPC FindNPCByName(string sNPCName)
        {
            foreach (Actor actor in NPCDictionary.Keys)
            {
                if (actor.Name.Equals(sNPCName))
                {
                    return NPCDictionary[actor];
                }
            }
            return null;
        }

        public NPC FindNPCByActor(Actor actor)
        {
            if (NPCDictionary.ContainsKey(actor))
            {
                return NPCDictionary[actor];
            }
            else
            {
                return null;
            }
        }

        public bool CheckChatArea(string sNPCName)
        {
            NPC npcTemp = FindNPCByName(sNPCName);
            if (null != npcTemp)
            {
                return npcTemp.CheckChatArea();
            }
            else
            {
                return false;
            }
        }

        private const string NPC_NAMEHEAD = "NPC_";

        private Dictionary<Actor, NPC> m_vNPC;       //<NPCActor,NPC>
    }

}