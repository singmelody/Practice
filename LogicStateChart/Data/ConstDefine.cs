using System;
using System.Runtime.CompilerServices;
using ScriptRuntime;

namespace RPGData
{
    public class ConstDefine
    {
        //主角
        public const string PLAYER_IDLE_ANIMATION = "asset:Role/1_Male_LongQiShi_01/Ready.anim";
        public const string PLAYER_RUN_ANIMATION = "asset:Role/1_Male_LongQiShi_01/Qian_Pao.anim";
        public const string PLAYER_READYCOMBAT_ANIMATION = "asset:Role/1_Male_LongQiShi_01/Ready.anim";
        public const string PLAYER_ATTACK_ANIMATION = "asset:Role/1_Male_LongQiShi_01/PuTongGJ_02.anim";
        public const string PLAYER_COMBATIDLE_ANIMATION = "asset:Role/1_Male_LongQiShi_01/Ready.anim";
        public const string PLAYER_HURT_ANIMATION = "asset:Role/1_Male_LongQiShi_01/ShouJi_01.anim";
        public const string PLAYER_DIE_ANIMATION = "asset:Role/1_Male_LongQiShi_01/SiWang.anim";
        public const string PLAYER_ACTORNAME = "1_Male_LongQiShi_01";
        public const float PLAYER_RUN_SPEED = 15.0f;
        public const int PLAYER_MAXHP = 5;
        public const int PLAYER_ATTACKDAMAGE = 2;
        public const string PLAYER_LEFTBBCOLLIDER = "Player_LeftBBCollider";
        public const string PLAYER_MIDDLEBBCOLLIDER = "Player_MiddleBBCollider";
        public const string PLAYER_RIGHTBBCOLLIDER = "Player_RightBBCollider";

        //敌人
        public const string ENEMY_IDLE_ANIMATION = "asset:Role/1_Male_LongQiShi_01/Ready.anim";
        public const string ENEMY_RUN_ANIMATION = "asset:Role/1_Male_LongQiShi_01/Qian_Pao.anim";
        public const string ENEMY_READYCOMBAT_ANIMATION = "asset:Role/1_Male_LongQiShi_01/Ready.anim";
        public const string ENEMY_ATTACK_ANIMATION = "asset:Role/1_Male_LongQiShi_01/PuTongGJ_02.anim";
        public const string ENEMY_COMBATIDLE_ANIMATION = "asset:Role/1_Male_LongQiShi_01/Ready.anim";
        public const string ENEMY_HURT_ANIMATION = "asset:Role/1_Male_LongQiShi_01/ShouJi_01.anim";
        public const string ENEMY_DIE_ANIMATION = "asset:Role/1_Male_LongQiShi_01/SiWang.anim";
        public const string ENEMY_ACTORNAME = "2_Male_LongQiShi_01";
        public const int ENEMY_MAXHP = 5;
        public const int ENEMY_ATTACKDAMAGE = 1;
        public const string ENEMY_LEFTBBCOLLIDER = "Enemy_LeftBBCollider";
        public const string ENEMY_MIDDLEBBCOLLIDER = "Enemy_MiddleBBCollider";
        public const string ENEMY_RIGHTBBCOLLIDER = "Enemy_RightBBCollider";
		public const float ENEMY_GUARD_RANGE = 15.0f;
		public const float ENEMY_VIEW_RANGE = 0.866f; // cos30

        //NPC对话区域
        public const int X_DISTANCE_NPC_CHAT = 20;
        public const int Z_DISTANCE_NPC_CHAT = 20;

    }

    public enum CharacterState
    {
        None = 0,
        Idle = 1,
        Running = 1 << 1,
        ReadyCombat = 1 << 2,
        Attack = 1 << 3,
        CombatIdle = 1 << 4,
        Hurt = 1 << 5,
        Die = 1 << 6
    }

    //NPC对话，说话者（0：玩家、1：NPC）
    public enum NPCChatSpeakerID
    {
        Player = 0,
        NPC = 1
    }

}