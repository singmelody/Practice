using System;
using System.Collections.Generic;
using System.Runtime.CompilerServices;
using ScriptRuntime;

namespace Logic
{
    public class Sound
    {
        public Sound(Actor root)
        {
            m_RootActor = root;
            m_vSoundUnits = new List<Actor>();
        }

        public void Play()
        {
            foreach (Actor actor in SoundUnitList)
            {
                actor.GetComponent<SoundSource>().Stop();
                actor.GetComponent<SoundSource>().Play();
            }
        }

        public void Play(Actor parentActor)
        {
            if (RootActor.Parent.Equals(parentActor))
            {
                Play();
            }
        }

        public void Stop()
        {
            foreach (Actor actor in SoundUnitList)
            {
                if (actor.GetComponent<SoundSource>().IsPlaying )
                {
                    actor.GetComponent<SoundSource>().Stop();
                }
            }
        }

        public void Pause()
        {
            foreach (Actor actor in SoundUnitList)
            {
                if (actor.GetComponent<SoundSource>().IsPlaying)
                {
                    actor.GetComponent<SoundSource>().Pause();
                }
            }
        }

        public void Active()
        {
            foreach (Actor actor in SoundUnitList)
            {
                if (!actor.IsActive)
                {
                    actor.ActiveWithoutChildren();
                }
            }
        }

        public void Init()
        {
            int iCount = ActorManager.GetActiveActorCount();
            for (int ii = 0; ii < iCount; ++ii)
            {
                Actor actor = ActorManager.GetActiveActor(ii);
                if (null == actor)
                {
                    throw (new ArgumentException("Sound.Init"));
                }

                if (actor.Name.StartsWith(RootActor.Name, true, null)
                    && actor.Name.Length > RootActor.Name.Length
                    && actor.Parent.Equals(RootActor))
                {
                    AddSoundUnit(actor);
                    actor.GetComponent<SoundSource>().Stop();
                }
            }
        }

        public void Clear()
        {
            m_vSoundUnits.Clear();
        }

        public Actor RootActor
        {
            get
            {
                return m_RootActor;
            }
        }

        public List<Actor> SoundUnitList
        {
            get
            {
                return m_vSoundUnits;
            }
        }

        private bool AddSoundUnit(Actor actor)
        {
            if (!m_vSoundUnits.Contains(actor))
            {
                m_vSoundUnits.Add(actor);
                return true;
            }

            return false;
        }

        private Actor m_RootActor;
        private List<Actor> m_vSoundUnits;
    }

	public class SoundMgr : Singleton<SoundMgr>
	{
        public const string SOUND_PLAYERHURT_NAMEHEAD = "4_Sound_Zhujue_Hurt";
        public const string SOUND_ENEMYHURT_NAMEHEAD = "4_Sound_NPC_Hurt";

        public const string SOUND_PLAYERATTACK_NAMEHEAD = "4_Sound_Zhujue_Attack";
        public const string SOUND_ENEMYATTACK_NAMEHEAD = "4_Sound_NPC_Attack";

        public const string SOUND_PLAYERDIE_NAMEHEAD = "4_Sound_Zhujue_Die";
        public const string SOUND_ENEMYDIE_NAMEHEAD = "4_Sound_NPC_Die";


        public SoundMgr()
        {
            m_vSounds = new Dictionary<Actor, Sound>();
            Load();
        }

        private void LoadOneNameSound(string sSoundRootName)
        {
            int iCount = ActorManager.GetActiveActorCount();
            for (int ii = 0; ii < iCount; ++ii)
            {
                Actor SoundRoot = ActorManager.GetActiveActor(ii);
                if (null == SoundRoot)
                {
                    throw (new ArgumentException("Sound.LoadOneNameSound"));
                }

                if (SoundRoot.Name.Equals(sSoundRootName))
                {
                    Sound sound = new Sound(SoundRoot);
                    sound.Init();
                    m_vSounds.Add(SoundRoot, sound);
                }
            }
        }

        private void Load()
        {
            LoadOneNameSound(SOUND_PLAYERATTACK_NAMEHEAD);
			LoadOneNameSound(SOUND_ENEMYATTACK_NAMEHEAD);
        }

        public void Init() { }

        public void PlaySound(string sSoundName)
        {
            foreach (KeyValuePair<Actor, Sound> pair in SoundDictionary)
            {
                if (pair.Key.Name.Equals(sSoundName))
                {
                    pair.Value.Play();
                }
            }
        }

        public void PlaySound(string sSoundName, Actor parentActor)
        {
            foreach (KeyValuePair<Actor, Sound> pair in SoundDictionary)
            {
                if (pair.Key.Parent.Equals(parentActor) && pair.Key.Name.Equals(sSoundName))
                {
                    pair.Value.Play();
                }
            }
        }
		
		private void Clear()
        {
            foreach (KeyValuePair<Actor, Sound> pair in SoundDictionary)
            {
                pair.Value.Clear();
            }
            SoundDictionary.Clear();
        }
		
		public void Reset()
        {
            Clear();
            Load();
        }

        public Dictionary<Actor, Sound> SoundDictionary
        {
            get
            {
                return m_vSounds;
            }
        }

        private Dictionary<Actor, Sound> m_vSounds;     

	}
}

