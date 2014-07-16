using System;
using System.Runtime.CompilerServices;
using ScriptRuntime;
using System.Collections.Generic;

namespace Logic
{
    public class Effect
    {
        public Effect(Actor root)
        {
            m_RootActor = root;
            m_vEffectUnits = new List<Actor>();
        }

        public void Play()
        {
            foreach (Actor actor in EffectUnitList)
            {
                actor.ParticleRender.GetParticleSystem().Stop();
                actor.ParticleRender.GetParticleSystem().Play();
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
            foreach (Actor actor in EffectUnitList)
            {
                if (actor.ParticleRender.GetParticleSystem().IsPlaying || actor.ParticleRender.GetParticleSystem().IsPaused)
                {
                    actor.ParticleRender.GetParticleSystem().Stop();
                }
            }
        }

        public void Pause()
        {
            foreach (Actor actor in EffectUnitList)
            {
                if (actor.ParticleRender.GetParticleSystem().IsPlaying)
                {
                    actor.ParticleRender.GetParticleSystem().Pause();
                }
            }
        }

        public void Active()
        {
            foreach (Actor actor in EffectUnitList)
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
                    throw (new ArgumentException("Effect.Init"));
                }

                if (actor.Name.StartsWith(RootActor.Name, true, null) 
                    && actor.Name.Length > RootActor.Name.Length
                    && actor.Parent.Equals(RootActor))
                {
                    AddEffectUnit(actor);
                    actor.ParticleRender.GetParticleSystem().Stop();
                }
            }
        }

        public void Clear()
        {
            m_vEffectUnits.Clear();
        }

        public Actor RootActor
        {
            get
            {
                return m_RootActor;
            }
        }

        public List<Actor> EffectUnitList
        {
            get
            {
                return m_vEffectUnits;
            }
        }

        private bool AddEffectUnit(Actor actor)
        {
            if (!m_vEffectUnits.Contains(actor))
            {
                m_vEffectUnits.Add(actor);
                return true;
            }

            return false;
        }

        private Actor m_RootActor;
        private List<Actor> m_vEffectUnits;
    }

    public class EffectMgr : Singleton<EffectMgr>
    {
        //public const string EFFECT_NAMEHEAD = "Effect_";
        public const string EFFECT_PLAYERHURT_NAMEHEAD = "3_Effect_zhujue_BJ";
        public const string EFFECT_ENEMYHURT_NAMEHEAD = "3_Effect_NPC_BJ";

        public EffectMgr()
        {
            m_vEffects = new Dictionary<Actor, Effect>();
            Load();
        }

        private void LoadOneNameEffect(string sEffectRootName)
        {
            int iCount = ActorManager.GetActiveActorCount();
            for (int ii = 0; ii < iCount; ++ii)
            {
                Actor effectRoot = ActorManager.GetActiveActor(ii);
                if (null == effectRoot)
                {
                    throw (new ArgumentException("Effect.LoadOneNameEffect"));
                }

                if (effectRoot.Name.Equals(sEffectRootName))
                {
                    Effect effect = new Effect(effectRoot);
                    effect.Init();
                    m_vEffects.Add(effectRoot, effect);
                }
            }
        }

        private void Load()
        {
            LoadOneNameEffect(EFFECT_PLAYERHURT_NAMEHEAD);
            LoadOneNameEffect(EFFECT_ENEMYHURT_NAMEHEAD);
        }

        private void Clear()
        {
            foreach (KeyValuePair<Actor, Effect> pair in EffectDictionary)
            {
                pair.Value.Clear();
            }
            EffectDictionary.Clear();
        }

        public void Init() { }

        public void Reset()
        {
            Clear();
            Load();
        }

        private void RemoveEffect(Actor actor)
        {
            if (m_vEffects.ContainsKey(actor))
            {
                m_vEffects.Remove(actor);
            }
        }

        public void PlayEffect(string sEffectName)
        {
            foreach (KeyValuePair<Actor, Effect> pair in EffectDictionary)
            {
                if (pair.Key.Name.Equals(sEffectName))
                {
                    pair.Value.Play();
                }
            }
        }

        public void PlayEffect(string sEffectName, Actor parentActor)
        {
            foreach (KeyValuePair<Actor, Effect> pair in EffectDictionary)
            {
                if (pair.Key.Parent.Equals(parentActor) && pair.Key.Name.Equals(sEffectName))
                {
                    pair.Value.Play();
                }
            }
        }

        public void PlayAll()
        {
            foreach (KeyValuePair<Actor, Effect> pair in EffectDictionary)
            {
                pair.Value.Play();
            }
        }

        public void StopEffect(string sEffectName)
        {
            foreach (KeyValuePair<Actor, Effect> pair in EffectDictionary)
            {
                if (pair.Key.Name.Equals(sEffectName))
                {
                    pair.Value.Stop();
                }
            }
        }

        public void StopEffect(string sEffectName, Actor parentActor)
        {
            foreach (KeyValuePair<Actor, Effect> pair in EffectDictionary)
            {
                if (pair.Key.Parent.Equals(parentActor) && pair.Key.Name.Equals(sEffectName))
                {
                    pair.Value.Stop();
                }
            }
        }

        public void StopAll()
        {
            foreach (KeyValuePair<Actor, Effect> pair in EffectDictionary)
            {
                pair.Value.Stop();
            }
        }

        public void PauseEffect(string sEffectName)
        {
            foreach (KeyValuePair<Actor, Effect> pair in EffectDictionary)
            {
                if (pair.Key.Name.Equals(sEffectName))
                {
                    pair.Value.Pause();
                }
            }
        }

        public void PauseEffect(string sEffectName, Actor parentActor)
        {
            foreach (KeyValuePair<Actor, Effect> pair in EffectDictionary)
            {
                if (pair.Key.Parent.Equals(parentActor) && pair.Key.Name.Equals(sEffectName))
                {
                    pair.Value.Pause();
                }
            }
        }

        public void PauseAll()
        {
            foreach (KeyValuePair<Actor, Effect> pair in EffectDictionary)
            {
                pair.Value.Pause();
            }
        }

        public Effect GetEffect(Actor actor)
        {
            if (m_vEffects.ContainsKey(actor))
            {
                return m_vEffects[actor];
            }

            return null;
        }

        public Dictionary<Actor, Effect> EffectDictionary
        {
            get
            {
                return m_vEffects;
            }
        }

        private Dictionary<Actor, Effect> m_vEffects;       //<特效根Actor，特效>
    }
}