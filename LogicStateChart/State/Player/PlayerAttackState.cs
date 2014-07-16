using System;
using System.Runtime.CompilerServices;
using ScriptRuntime;
using System.Collections.Generic;
using RPGData;

namespace Logic
{
    public class PlayerHurtState : Singleton<PlayerHurtState>, IState
    {
        public PlayerHurtState()
        {

        }

        // interface implement
        public void Enter(GameEntity entity)
        {
			Debug.Printf("Enter PlayerHurtState\n");

			entity.Data.AvatarHP -= ConstDefine.ENEMY_ATTACKDAMAGE;

            EffectMgr.Instance.PlayEffect(EffectMgr.EFFECT_PLAYERHURT_NAMEHEAD, entity.Data.AvatarActor);

            CameraMgr.Instance.m_bShakeCamera = true;
        }

        public void Exit(GameEntity entity)
        {
        }

        public void Execute(GameEntity entity)
        {
            bool bRet = false;
            IState state = null;
            if (entity.Data.IsDied)
            {
                if (entity.CanEnterCharacterState(PlayerDieState.Instance))
                {
                    state = PlayerDieState.Instance;
                    bRet = CommonUtility.CheckAnimation(entity, ConstDefine.PLAYER_DIE_ANIMATION);
                }
            }
            else
            {
                if (entity.CanEnterCharacterState(PlayerIdleState.Instance))
                {
                    state = PlayerIdleState.Instance;
                    bRet = CommonUtility.CheckAnimation(entity, ConstDefine.PLAYER_HURT_ANIMATION);
                }
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