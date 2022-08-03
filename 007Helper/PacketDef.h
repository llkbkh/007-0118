#pragma once
enum class UMsgType {

	Chat        = 0,		//私聊
	Notice		= 10,		//公告	_大厅提示框+游戏里中间绿色提示
	SysNotice   = 11,		//系统公告	_橘黄色大厅与游戏左下角 提示
	MsgStr      = 30,		//提示文本消息
	LIKE        = 31,		//点赞
	AdminNotice = 51,		//管理员公告
	ItemBoard	= 60,		//道具栏
	NoticeBoard = 65,		//公告栏
	Lottery     = 80,		//公屏中奖信息

};

enum class KickType {

	Error    = 0,			//错误信息
	ReMain   = 1,			//剩余投票
	KickInfo = 2,			//举报信息
	KickRoom = 3,			//被踢出房间
	KickInit = 4,			//初始化

};

enum class UpRoomType {

	RoomInfo        = 0,			//房间信息
	RoomPlayerCount = 4,	       //修改房间人数
	RoomGameEnd     = 6,           //游戏结算
	RoomKick        = 7,           //房主踢人
	RoomKickExp     = 0x11,        //被踢结算经验
	RoomWeaponRate  = 0x23,        //结束后武器打分

};


typedef struct GAME_PACKET_HEAD
{
	BYTE	Head;		//头部 固定 0x55
	BYTE	Seq;		//封包序列 0-255->0
	SHORT	Lenght;		//封包长度 (一个封包长度 -不包括头部四个字节)

	inline GAME_PACKET_HEAD()
	{
		RtlZeroMemory(this, sizeof(GAME_PACKET_HEAD));
		this->Head = 0x55;
	}
}TAG_GAME_PACKET_HEAD, * PTAG_GAME_PACKET_HEAD;






typedef struct _MEMORY_MAPPED_FILE_NAME_INFORMATION
{
	UNICODE_STRING Name;
	WCHAR Buffer[1024];
} MEMORY_MAPPED_FILE_NAME_INFORMATION, * PMEMORY_MAPPED_FILE_NAME_INFORMATION;
#define		EVP_MAX_IV_LENGTH   16
#define 	EVP_MAX_BLOCK_LENGTH   32
struct evp_cipher_ctx_st {
	const void* cipher;
	void* engine;             /* functional reference if 'cipher' is
								 * ENGINE-provided */
	int encrypt;                /* encrypt or decrypt */
	int buf_len;                /* number we have left */
	unsigned char oiv[EVP_MAX_IV_LENGTH]; /* original iv */
	unsigned char iv[EVP_MAX_IV_LENGTH]; /* working iv */
	unsigned char buf[EVP_MAX_BLOCK_LENGTH]; /* saved partial block */
	int num;                    /* used by cfb/ofb/ctr mode */
	/* FIXME: Should this even exist? It appears unused */
	void* app_data;             /* application stuff */
	int key_len;                /* May change for variable length cipher */
	unsigned long flags;        /* Various flags */
	void* cipher_data;          /* per EVP data */
	int final_used;
	int block_mask;
	unsigned char final[EVP_MAX_BLOCK_LENGTH]; /* possible final block */
};
typedef int(*PfnSSL_read)(void* ssl, void* buf, int num);

typedef int(*PfnEVP_EncryptUpdate)(VOID* ctx, unsigned char* out, int* outl, unsigned char* in, int inl);
typedef int(*PfnEVP_DecryptUpdate)(VOID* ctx, unsigned char* out, int* outl, const unsigned char* in, int inl);
typedef int(*PfnEVP_EncryptInit)  (VOID* ctx, const VOID* type, unsigned char* key, unsigned char* iv);
typedef int(WSAAPI* PfnSend)  (SOCKET s, const char* buf, int len, int flags);

extern PfnEVP_DecryptUpdate EVP_DecryptUpdate007;
extern PfnEVP_EncryptUpdate EVP_EncryptUpdate007;
extern PfnEVP_EncryptInit	EVP_EncryptInit007;
extern PfnSend				_Send;
extern PfnSSL_read			SSL_read007;