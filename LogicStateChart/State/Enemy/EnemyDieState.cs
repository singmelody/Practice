using System;
using System.Runtime.CompilerServices;
using ScriptRuntime;
using System.Collections.Generic;
using RPGData;

namespace Logic
{
    public class EnemyDieState : Singleton<EnemyDieState>, IState
    {
        public EnemyDieState()
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
            if (!entity.Data.AvatarActor.IsActive)
                return;

            bool bRet = CommonUtility.CheckAnimation(entity, ConstDefine.ENEMY_DIE_ANIMATION);
            if (bRet)
            {
                entity.Machine.ChangeState(EnemyIdleState.Instance);
                entity.Data.AvatarActor.DeactiveWithChildren();
            }

        }

        public bool OnMessage(GameEntity entity, Message msg)
        {
            return false;
        }
    }
}