using System;
using System.Runtime.CompilerServices;
using ScriptRuntime;
using RPGData;

namespace Logic
{
    public class PlayerBBCollider : BBCollider
    {
        public PlayerBBCollider(GameEntity entity)
        {
            Owner = entity;

			LeftBBCollider = new BBCollider();
			MiddleBBCollider = new BBCollider();
			RightBBCollider = new BBCollider();
        }

        public override void Init()
        {
            Actor leftActor = ActorManager.FindActiveActor(ConstDefine.PLAYER_LEFTBBCOLLIDER);
            if (null == leftActor)
            {
                throw (new ArgumentException("PlayerBBCollider.Init.Player_LeftBBCollider is null"));
            }
            Actor middleActor = ActorManager.FindActiveActor(ConstDefine.PLAYER_MIDDLEBBCOLLIDER);
            if (null == middleActor)
            {
                throw (new ArgumentException("PlayerBBCollider.Init.Player_MiddleBBCollider is null"));
            }
            Actor rightActor = ActorManager.FindActiveActor(ConstDefine.PLAYER_RIGHTBBCOLLIDER);
            if (null == rightActor)
            {
                throw (new ArgumentException("PlayerBBCollider.Init.Player_RightBBCollider is null"));
            }

            LeftBBCollider.RegistCallback( Owner, leftActor, BBCollideCallback);
            BBColliderMgr.Instance.Register(LeftBBCollider);

            RightBBCollider.RegistCallback( Owner, rightActor, BBCollideCallback);
            BBColliderMgr.Instance.Register(RightBBCollider);

            MiddleBBCollider.RegistCallback( Owner, middleActor, BBCollideHurtCallback);
            BBColliderMgr.Instance.Register(MiddleBBCollider);
        }

        private void BBCollideCallback(GameEntity entity, Actor boxOther)
        {

        }

        private void BBCollideHurtCallback(GameEntity enemy, Actor boxOther)
        {
            //判定是否敌人武器的包围盒
            if (boxOther.Name == ConstDefine.ENEMY_LEFTBBCOLLIDER
			    || boxOther.Name == ConstDefine.ENEMY_RIGHTBBCOLLIDER)
            {
                //判定条件
                //1、敌人是否在攻击状态
                //2、敌人本次攻击给玩家造成的伤害次数
                //3、敌人是否死亡
                if (enemy.Machine.CurrentState == EnemyAttackState.Instance
                    && enemy.Data.CheckDamageCountPerAttack()
                    && !enemy.Data.IsDied)
                {
                    //减血

                    if (Owner.CanEnterCharacterState(PlayerHurtState.Instance))
                    {
                        Message msg = new Message();
                        msg.state = PlayerHurtState.Instance;
                        MessageDispatcher.Instance.DispatchMessage(enemy, Owner, msg);
                    }

                    //增加攻击给玩家造成的伤害次数
                    enemy.Data.AddDamageCountPerAttack();
                }
            }
        }
    }
}