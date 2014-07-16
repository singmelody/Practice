using System;
using System.Runtime.CompilerServices;
using ScriptRuntime;
using System.Collections.Generic;
using RPGData;

namespace Logic
{
    public class AvatarData
    {
        public AvatarData()
        {
            AvatarActor = null;
            Init();
        }

        public AvatarData(Actor actor)
        {
            AvatarActor = actor;
            Init();
        }

        public virtual void Init()
        {
            AvatarHP = 0;
            IsAutoMove = false;
            m_vNavigationPath = new List<Vector3>();
            ResetDamageCountPerAttack();
        }

        public virtual void Reset()
        {
            if (!AvatarActor.IsActive)
            {
                AvatarActor.ActiveWithChildren();
            }
            AvatarHP = 0;
            IsAutoMove = false;
            NavigationPathList.Clear();
            ResetDamageCountPerAttack();
        }

        public virtual void Clear()
        {
            NavigationPathList.Clear();
        }

        public Actor AvatarActor
        {
            get
            {
                return m_Actor;
            }
            set
            {
                m_Actor = value;
            }
        }

        public int AvatarHP
        {
            get
            {
                return m_iHP;
            }
            set
            {
                m_iHP = value;
            }
        }

        public Vector3 AvatarDir
        {
            get
            {
                return m_Dir;
            }
            set
            {
                m_Dir = value;
            }
        }

        public bool IsDied
        {
            get
            {
                return 0 >= AvatarHP;
            }
        }

        public bool IsAutoMove
        {
            get
            {
                return m_bIsAutoMove;
            }
            set
            {
                m_bIsAutoMove = value;
            }
        }

        public List<Vector3> NavigationPathList
        {
            get
            {
                return m_vNavigationPath;
            }
            set
            {
                m_vNavigationPath = value;
            }
        }

        private uint DamageCountPerAttack
        {
            get
            {
                return m_iDamageCountPerAttack;
            }
            set
            {
                m_iDamageCountPerAttack = value;
            }
        }

        public bool CheckDamageCountPerAttack()
        {
            return 0 == DamageCountPerAttack;
        }

        public void AddDamageCountPerAttack()
        {
            ++DamageCountPerAttack;
        }

        public void ResetDamageCountPerAttack()
        {
            DamageCountPerAttack = 0;
        }

        private Actor m_Actor;
        private int m_iHP;

        private bool m_bIsAutoMove;     //是否正在自动寻路
        private List<Vector3> m_vNavigationPath;        //路点集合
		
		public Vector3 m_PlayerDir = new Vector3(0, 0, 1.0f);

        private uint m_iDamageCountPerAttack;        //每次攻击给对手造成的伤害次数
        private Vector3 m_Dir = new Vector3(0, 0, 1.0f);

        protected BBCollider m_BBCollider;
    }
}