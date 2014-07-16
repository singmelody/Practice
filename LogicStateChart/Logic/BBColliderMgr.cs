using System;
using System.Runtime.CompilerServices;
using ScriptRuntime;
using System.Collections.Generic;

namespace Logic
{
    public class BBCollider
    {
        public delegate void BBColliderCallback(GameEntity entity, Actor boxOther);

        public BBCollider()
        {
        }

        public virtual void Init()
        {

        }

        public virtual void RegistCallback(GameEntity entity, Actor boxActor, BBColliderCallback callback)
        {
            ColliderEntity = entity;
            BoxActor = boxActor;
            m_Callback = callback;
        }

        public void Call(GameEntity entity, Actor boxOther)
        {
            if (null != m_Callback)
            {
                m_Callback(entity, boxOther);
            }
        }

        public Actor BoxActor
        {
            get
            {
                return m_ColliderActor;
            }
            private set
            {
                m_ColliderActor = value;
            }
        }

        protected BBCollider LeftBBCollider;
        protected BBCollider MiddleBBCollider;
        protected BBCollider RightBBCollider;

        protected GameEntity Owner;

		public GameEntity ColliderEntity;
        private BBColliderCallback m_Callback;
        private Actor m_ColliderActor;

    }

    public class BBColliderMgr : Singleton<BBColliderMgr>
    {
		public void Init() { }

        public BBColliderMgr()
        {
            Colliders = new List<BBCollider>();
        }

        private List<BBCollider> Colliders
        {
            get
            {
                return m_vColliders;
            }
            set
            {
                m_vColliders = value;
            }
        }

        public void Register(BBCollider collider)
        {
            if (!Colliders.Contains(collider))
            {
                Colliders.Add(collider);
            }
        }

        public void Clear()
        {
            Colliders.Clear();
        }

        public void Tick()
        {
            int iCollidersCount = Colliders.Count;
            for (int ii = 0; ii < iCollidersCount; ++ii)
            {
                for (int jj = ii + 1; jj < iCollidersCount; ++jj)
                {
                    BoundingBox bbi = Colliders[ii].BoxActor.WorldBoundingBox;
                    BoundingBox bbj = Colliders[jj].BoxActor.WorldBoundingBox;

                    if (ClipStatus.Outside != bbi.Contains(bbj) && ClipStatus.Outside != bbj.Contains(bbi))
                    {
                        AvatarData data = Colliders[jj].ColliderEntity.Data;

                        if (data.AvatarActor.IsActive)
                        {
                            Colliders[ii].Call(Colliders[jj].ColliderEntity, Colliders[jj].BoxActor);
                        }

                        if (data.AvatarActor.IsActive)
                        {
                            Colliders[jj].Call(Colliders[ii].ColliderEntity, Colliders[ii].BoxActor);
                        }
                    }
                }
            }
        }

        private List<BBCollider> m_vColliders;
    }
}