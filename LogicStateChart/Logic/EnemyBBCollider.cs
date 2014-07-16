using System;
using System.Runtime.CompilerServices;
using ScriptRuntime;
using RPGData;

namespace Logic
{
    public class EnemyBBCollider : BBCollider
    {
        public EnemyBBCollider(Enemy enemy)
        {
            Owner = enemy;

			LeftBBCollider = new BBCollider();
			MiddleBBCollider = new BBCollider();
			RightBBCollider = new BBCollider();
        }

        public override void Init ()
		{
			int num = Owner.Data.AvatarActor.GetChildCount();
			for (int i = 0; i < num; ++i) 
			{
                Actor temp = Owner.Data.AvatarActor.GetChild(i);
				if ( null == temp )
				{
					throw (new ArgumentException("EnemyBBCollider.Init.Enemy_BBCollider is null"));
				}
				if ( temp.Name == ConstDefine.ENEMY_LEFTBBCOLLIDER )
				{
                    LeftBBCollider.RegistCallback( Owner, temp, BBCollideCallback);
					BBColliderMgr.Instance.Register(LeftBBCollider);
				}
				else if (temp.Name == ConstDefine.ENEMY_MIDDLEBBCOLLIDER )
				{
                    MiddleBBCollider.RegistCallback( Owner, temp, BBCollideHurtCallback);
					BBColliderMgr.Instance.Register(MiddleBBCollider);
				}
				else if ( temp.Name == ConstDefine.ENEMY_RIGHTBBCOLLIDER)
				{
                    RightBBCollider.RegistCallback( Owner, temp, BBCollideCallback);
					BBColliderMgr.Instance.Register(RightBBCollider);
				}
			}
        }

        private void BBCollideCallback(GameEntity entity, Actor boxOther)
        {

        }

        private void BBCollideHurtCallback(GameEntity entity, Actor boxOther)
        {
            //判定是否主角武器的包围盒
            if (boxOther.Name == ConstDefine.PLAYER_LEFTBBCOLLIDER
                || boxOther.Name == ConstDefine.PLAYER_RIGHTBBCOLLIDER)
            {
                //判定条件
                //1、主角是否在攻击状态
                //2、主角本次攻击给怪物造成的伤害次数=0 开始攻击,表示主角已经不干了怪物了,开始干主角
                //3、主角是否死亡
				if( (entity.Machine.CurrentState == PlayerAttackState.Instance
				     || entity.Machine.CurrentState == PlayerRunAttackState.Instance)
				   && entity.Data.CheckDamageCountPerAttack()
				   && !entity.Data.IsDied
                    )
                {
					//减血
					
					Message msg = new Message();
					msg.state = EnemyHurtState.Instance;
					MessageDispatcher.Instance.DispatchMessage( null, Owner, msg);
					
					//增加攻击给主角造成的伤害次数
					entity.Data.AddDamageCountPerAttack();
                }
				else if( entity.Data.IsDied)
				{
                    Message msg = new Message();
                    msg.state = EnemyIdleState.Instance;
                    MessageDispatcher.Instance.DispatchMessage( null, Owner, msg);
				}
            }
        }
    }
}