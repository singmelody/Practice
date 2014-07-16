using System;
using System.Runtime.CompilerServices;
using ScriptRuntime;
using System.Collections.Generic;
using RPGData;

namespace Logic
{
    public class EnemyAttackState : Singleton<EnemyAttackState>, IState
    {
        public EnemyAttackState()
        {
        }

        // interface implement
        public void Enter(GameEntity entity)
        {
            SoundMgr.Instance.PlaySound(SoundMgr.SOUND_ENEMYATTACK_NAMEHEAD, entity.Data.AvatarActor);
        }

        public void Exit(GameEntity entity)
        {
        }

        public void Execute(GameEntity entity)
        {
            bool b = CommonUtility.CheckAnimation(entity, ConstDefine.ENEMY_ATTACK_ANIMATION);
            if (b)
                entity.Machine.ChangeState(EnemyCombatIdleState.Instance);
        }

        public bool OnMessage(GameEntity entity, Message msg)
        {
            return false;
        }
    }

    public class EnemyReadyCombatState : Singleton<EnemyReadyCombatState>, IState
    {
        public EnemyReadyCombatState()
        {

        }

        // interface implement
        public void Enter(GameEntity entity)
        {
        }

        public void Exit(GameEntity entity)
        {
        }

        public void Execute(GameEntity entity)
        {
            bool b = CommonUtility.CheckAnimation( entity, ConstDefine.ENEMY_READYCOMBAT_ANIMATION);
            if (b)
                entity.Machine.ChangeState(EnemyAttackState.Instance);
        }

        public bool OnMessage(GameEntity entity, Message msg)
        {
            return false;
        }
    }

    public class EnemyCombatIdleState : Singleton<EnemyCombatIdleState>, IState
    {
        public EnemyCombatIdleState()
        {
        }

        // interface implement
        public void Enter(GameEntity entity)
        {
        }

        public void Exit(GameEntity entity)
        {
        }

        public void Execute(GameEntity entity)
        {
            bool b = CommonUtility.CheckAnimation(entity, ConstDefine.ENEMY_COMBATIDLE_ANIMATION);
            if (b)
                entity.Machine.ChangeState(EnemyAttackState.Instance);
        }

        public bool OnMessage(GameEntity entity, Message msg)
        {
            return false;
        }
    }

    public class EnemyHurtState : Singleton<EnemyHurtState>, IState
    {
        public EnemyHurtState()
        {
        }

        // interface implement
        public void Enter(GameEntity entity)
        {
            // minute hp
			Debug.Printf("Enemy HP Minute\n");
            entity.Data.AvatarHP -= ConstDefine.PLAYER_ATTACKDAMAGE;
            EffectMgr.Instance.PlayEffect(EffectMgr.EFFECT_ENEMYHURT_NAMEHEAD, entity.Data.AvatarActor);
        }

        public void Exit(GameEntity entity)
        {
        }

        public void Execute (GameEntity entity)
		{
			bool bRet = false;
            IState state = null;

			if (entity.Data.IsDied) 
			{
                state = EnemyDieState.Instance;
				bRet = CommonUtility.CheckAnimation (entity, ConstDefine.ENEMY_DIE_ANIMATION);
			} 
			else 
			{
                state = EnemyAttackState.Instance;
				bRet = CommonUtility.CheckAnimation (entity, ConstDefine.ENEMY_HURT_ANIMATION);
			}

			if (bRet) 
                entity.Machine.ChangeState(state);
        }

        public bool OnMessage(GameEntity entity, Message msg)
        {
            return false;
        }
    }
}