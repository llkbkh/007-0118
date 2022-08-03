#include "RCFServer.h"
#include <RCF/ProxyEndpoint.hpp>
#include "RCF/ClientProgress.hpp"
#include "RCF/Enums.hpp"
#include <PackWrite.h>



CRCFServerManage::CRCFServerManage()
{
	m_pRcfInit     = nullptr;
	m_uPort        = 0;
	m_pSagaServer  = nullptr;
	m_pServer      = nullptr;
	m_pInstance    = this;
}

CRCFServerManage* CRCFServerManage::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new CRCFServerManage();
	}
	return m_pInstance;
}

BOOL CRCFServerManage::Init(ULONG uPort)
{
	m_pRcfInit = new RCF::RcfInit();
	m_pSagaServer = new SagaService();
	try
	{
		m_pServer = new RCF::RcfServer(RCF::TcpEndpoint(xorstr_("0.0.0.0"), uPort));
		//启用代理端点
		m_pServer->setEnableProxyEndpoints(true);
		//设置线程池
		m_pServer->setThreadPool(RCF::ThreadPoolPtr(new RCF::ThreadPool(1, 10)));
		//绑定
		auto Ptr =  m_pServer->bind<I_SagaService>(*m_pSagaServer);
		//方法控制
		auto accessControl = [&](int methodId) { return onServiceAccess(methodId); };
		Ptr->setAccessControl(accessControl);
		//设置最大连接数
		//-最高100开客户端 NGS+游戏
		m_pServer->getServerTransport().setConnectionLimit(200);
		//最大传输长度 -最大5MB
		m_pServer->getServerTransport().setMaxIncomingMessageLength(5 * 1024 * 1024);
		m_pServer->start();

	}
	catch (const RCF::Exception& e)
	{
		m_StrError = e.getErrorMessage();

		return 0;
	}

	return m_pServer->isStarted();
}

bool CRCFServerManage::onServiceAccess(int methodId)
{
	bool bRet = false;
	//VMProtectBegin(__FUNCTION__);

	if (methodId == 0)
	{
		//methodId = 0 版本验证
		return true;
	}
	RCF::RcfSession & ConnectSession = RCF::getCurrentRcfSession();
	SagaServiceSession& SagaSession = ConnectSession.getSessionObject<SagaServiceSession>(true);
	bRet = !SagaSession.GetSessionID().empty();

	ACEDebugFileLog("[{}] MethodID:{} Session:{}  Ret:{}",__FUNCTION__, methodId, SagaSession.GetSessionID(), bRet);


	if (bRet)
	{
		//验证 session
		std::string ClientSession = ConnectSession.getRequestUserData();

		if (SagaSession.GetSessionID() != ClientSession)
		{
			ACEErrorFileLog("[{}] Session Check Fail MethID :{}",__FUNCTION__,methodId);

			return false;
		}

	}
	
	//VMProtectEnd();
	return bRet;
}

std::vector<BYTE> SagaService::SRC4Decode(std::vector<BYTE> CipherByte, std::string RC4Key)
{
	RC4_KEY s_table;
	RC4_set_key(&s_table, RC4Key.length(), (unsigned char*)RC4Key.c_str());
	std::vector<BYTE>DecodeText(CipherByte.size());
	RC4(&s_table, CipherByte.size(), (unsigned char*)CipherByte.data(), DecodeText.data());     //解密
	return DecodeText;
}

std::vector<BYTE> SagaService::SRC4Encrypt(std::vector<BYTE> CipherByte, std::string RC4Key)
{
	RC4_KEY s_table;
	RC4_set_key(&s_table, RC4Key.length(), (unsigned char*)RC4Key.c_str());
	std::vector<BYTE> Rc4Data(CipherByte.size());
	RC4(&s_table, CipherByte.size(), (unsigned char*)CipherByte.data(), Rc4Data.data());		//加密
	return Rc4Data;
}

std::string& CRCFServerManage::GetStrError()
{
	return m_StrError;
}

RCF::RcfServer* CRCFServerManage::GetRcfServerPoInter()
{
	return m_pServer;
}

BOOL CRCFServerManage::SendClientMsg(std::string ClientID, std::string Umsg)
{
	RcfClient<I_SagaProxy> client(RCF::ProxyEndpoint(*m_pServer, ClientID));
	std::vector<BYTE> VecByte(1024 + Umsg.length(), 0);
	auto lSendPack = std::make_shared<PackWriter>(VecByte);
	//msg type 
	lSendPack->WriteByte(1);
	lSendPack->WriteString(Umsg);



	RCF::Future<std::vector<BYTE>> fRet = client.UMSG(VecByte);
	fRet.wait(2000);

	std::unique_ptr<RCF::Exception> ePtr = client.getClientStub().getAsyncException();


	//if (ePtr)
	//{
	//	ACEErrorFileLog("{}", ePtr->getErrorString());
	//}
	//else
	//{
	//	ACEDebugFileLog("[{}] Code:{}", __FUNCTION__, "ok");
	//}
	return ePtr == nullptr;
}

void CRCFServerManage::SendAllClientMsg(std::string Umsg)
{
	auto Test = [=]()
	{
		CACEUtil Util;
		std::vector<std::string> proxyEndpointNames;
		CRCFServerManage::GetInstance()->GetRcfServerPoInter()->enumerateProxyEndpoints(proxyEndpointNames);

		for (auto iter : proxyEndpointNames)
		{
			CRCFServerManage::GetInstance()->SendClientMsg(iter, Umsg);
		}
	};

	std::thread t(Test);
	t.detach();
}

void onClientDisconnect(RCF::RcfSession& session)
{
	//断开回调
}
void onRemoteCallProgress(const  RCF::RemoteCallProgressInfo& info, RCF::RemoteCallAction& action)
{

	ACEDebugFileLog("[{}] {} {} {}" ,__FUNCTION__, info.mBytesTransferred, info.mBytesTotal, (ULONG)info.mPhase);
}



SagaService::SagaService()
{
	m_RC4Key = xorstr_("99BA5433DF5FA898C8E078B8BA55F251");
	m_Count  = 0;
	m_RSAPri = xorstr_(R"(-----BEGIN PRIVATE KEY-----
MIIEvgIBADANBgkqhkiG9w0BAQEFAASCBKgwggSkAgEAAoIBAQDHwmquia1IC+g3
3FkYu0whv575vS+HwsFUIOA9cjTRwDNM2CAyZgLWaaIea+1tHV0StFTBTFLYk53+
KokE8Vq/7rYiZ6gp0XheEWds7Pj9SaMaa+PbjRBTZKrhBv2NHAMA8XL2XagMCCAH
YaYA34M3Iv+l7WUGpwjqNV4st55By1VtQ8Wn6qqC+GgIwcfLv5jq96Iv3IScSsN/
GD/dwLzvJW8KlJPKA8tYsj/kbYtVKTbUBoTzrdAuYo7+wyBsTJnwUHKeuFybKvWR
Ob3qDmRaJ4NxDBVbVK99x47bywWFc9D8Jtdd+S1MED4LCYUupv/9ZDf9NhSrTa2V
PA9M7i5jAgMBAAECggEALZua5dyzPnbRfyeFBBuHR1FLx7NFDdcOctnKuNzBaH5c
eBgESd12mmOlyxJTNA13OZUIoNV/OMmDUgt+Gy3LzXSACxzljR6xP7DzjzAPrSzJ
23d0n6T2bEPYq+X/r98MZIO7/rnla1HTljvaPerKN6V6/vRTk5BBfc/Pf6Awts6B
ceo+Wg5yQHzSxa+QGe9dU3LGNFRMUL6Yn4oDyEeJdLQEcSVuaGL/yjjguDG2VOqv
xUg9OIi+RSTxhklRuMNCchZahwWXBPtQ4ITim8mLE0YC+W8ID5N2momC+Vgw5glk
/URQphp8Xqrid00GFzpFFyN/6r9txCQzj2OgOVHBOQKBgQDv9WR/ZpKFYRVcY3/y
O4d13yfsfYuVOV3Z0yqd4KhB6/RVS5muX8zYlynsQK0/6J48Z267dAHNB5j5RPuR
GPY0ybRIlIjWkWWTv+0ppZIDCEUx5VjqB2z7wCJ89V50lX5Ex3uvMZolwI66DVx2
Ez5pAqZBh/5pWY50NXd8+frfDwKBgQDVHRBJak5wZTniusHdEM0ztp/letcxyQP8
gZKoz2ARjJ6GwswpkAkPK4cUKh3nxWxTENZ/5nZuQUSwV9ndwE3fEAv+j0pmojSO
1XXt2uvqS+2+Nq+RY4z84ZasMLmezwlkvcp8Fc1kBAiWe4DcL9AyFNtwuxEqXCq1
BuJUnP97bQKBgQCrVDHKSSlqtFO1IJH1GY3xmkD+OPGVvV0GLyDpksNwNhHBaPY0
q9AuAVh8h/1reSKqDEvi6cRzaO5PZHKeOg1oyW/AttUxJqU5JjsmlkI3igslwhSH
HIAzDA9T2zny0d5TK39jKm5ecCs4KlwPLlRMVAjzFy+OdtQgoyBKio4V/wKBgFbV
O8CeFw3GwPo33CvF38Z1X9sx3SLE4+HrraUosd0Urbb9AhDhRj+vHZ/CtWKtV0DV
OIDul0y/PUyYzwWCiHgd4rU7DzaYWs/j++QJclyJyXnacFjN9jFHfYv1YnxpoUsx
5aH/hCt/VTxzYALXXH47b1oUbp8S5LfT8T5GpfZVAoGBAMnC7zB61lBCbCDOE4Lz
N08o38120/IrSfYnO2EdxUkqQNJXRLsYrj43Pn4U+GF6PZJqQt2hxmnmcFBTm50T
1tREJ0PVWaQyOojVAUpOyqPpuSWRx82cWuw/VXmFt6c3YMJ2ozA9GZ9cY7iOMjzx
AykutLAFWhJ9UlSRHJ9y2j01
-----END PRIVATE KEY-----)");

	m_MagicNumber = 0x89252061;
}



BOOL SagaService::Version(std::vector<BYTE>& v)
{
	VMProtectBegin(__FUNCTION__);

	BOOL bRet = FALSE;

	auto Text =  RsaPriDecrypt(v,m_RSAPri);

	if (Text.size() <= 0)
	{
		ACEDebugFileLog("[{}] RSA私钥解密失败",__FUNCTION__);
		return bRet;
	}

	auto lPackRead = std::make_shared<PackReader>(Text);

	int Number = lPackRead->ReadChar();
	if (Number != 0x7E)
	{
		ACEDebugFileLog("[{}] Head error:{:X}", __FUNCTION__, Number);
		return bRet;
	}
	int Value = lPackRead->ReadInt();
	if (Value != m_MagicNumber)
	{
		ACEDebugFileLog("[{}] MagicNumber error:{:X}", __FUNCTION__, Value);
		return bRet;
	}
	//客户端发来的随机数盐
	int Salt = lPackRead->ReadInt();
	//验证成功.下发RC4 秘钥
	RCF::RcfSession& ConnectSession = RCF::getCurrentRcfSession();
	SagaServiceSession& SagaSession = ConnectSession.getSessionObject<SagaServiceSession>(true);

	//随机生成RC4秘钥
	std::string & RC4Key = SagaSession.GenerateRC4Key(m_MagicNumber);

	//Session
	std::string& Session = SagaSession.CreteSession();

	//RSA加密数据
	auto lSendPack = std::make_shared<PackWriter>(2048);
	//head
	lSendPack->WriteByte(0x7E);
	//salt
	lSendPack->WriteInt(Salt);
	//Session
	lSendPack->WriteString(Session);
	//RC4 Key
	lSendPack->WriteString(RC4Key);

	std::string v1 = std::string((char*)lSendPack->GetBytes(), lSendPack->GetBytesLength());

	auto result = RsaPriEncrypt(v1,m_RSAPri);

	if (!result.size())
	{
		ACEDebugFileLog("[{}] RSA加密失败", __FUNCTION__);
		return bRet;
	}
	RtlZeroMemory(v.data(), v.size());

	std::copy(result.begin(), result.end(), v.begin());

	bRet = true;

	VMProtectEnd();
	return bRet;
}


std::vector<BYTE> SagaService::ClientInfo(std::vector<BYTE>& v)
{
	RCF::RcfSession& ConnectSession = RCF::getCurrentRcfSession();
	SagaServiceSession& SagaSession = ConnectSession.getSessionObject<SagaServiceSession>(true);

	auto VecDecode = SRC4Decode(v, SagaSession.GetRC4Key());
	auto lPackRead = std::make_shared<PackReader>(VecDecode);

	auto ProcessName    =	lPackRead->ReadStr();
	auto ProcessVersion =	lPackRead->ReadStr();

	std::string Result;
	BOOL bRet = FALSE;


	if (SagaSession.GetClientInfo().count(ProcessName) <= 0)
	{
		Result = fmt::format(xorstr_("Not found Client Process Name :{}"), ProcessName);
		ACEDebugFileLog("[{}] {}", __FUNCTION__, Result);
		goto Lab_Exit;
	}
	else if (SagaSession.GetClientInfo().at(ProcessName) != ProcessVersion)
	{
		//比对版本
		Result = fmt::format(xorstr_("Version mismatch :{}"), ProcessVersion);
		ACEDebugFileLog("[{}] {}", __FUNCTION__, Result);
		goto Lab_Exit;
	}
	bRet = TRUE;
	Result = xorstr_("ok");
Lab_Exit:
	std::vector<BYTE> VecResult(1024 + Result.length(), 0);

	auto lPackWrite = std::make_shared<PackWriter>(VecResult);

	lPackWrite->WriteByte(bRet);

	lPackWrite->WriteString(Result);

	return SRC4Encrypt(VecResult, SagaSession.GetRC4Key());
}

std::vector<BYTE> SagaService::RemoteVar(std::vector<BYTE>& v)
{
	RCF::RcfSession& ConnectSession = RCF::getCurrentRcfSession();
	SagaServiceSession& SagaSession = ConnectSession.getSessionObject<SagaServiceSession>(true);



	auto VecDecode = SRC4Decode(v, SagaSession.GetRC4Key());
	auto lPackRead = std::make_shared<PackReader>(VecDecode);

	auto RemoteVar =  lPackRead->ReadStr();


	auto  pVar = FyAuthorization::GetInstanc()->RemoteVariable(RemoteVar);
	//成功返回空字符串 否则为错误信息
	std::string Result = std::get<1>(pVar);
	BOOL bRet          = std::get<0>(pVar);

	std::vector<BYTE> VecResult(1024 + Result.length(), 0);

	auto lPackWrite = std::make_shared<PackWriter>(VecResult);

	lPackWrite->WriteByte(bRet);

	lPackWrite->WriteString(Result);

	return SRC4Encrypt(VecResult, SagaSession.GetRC4Key());
}

std::vector<BYTE> SagaService::ClientCheck(std::vector<BYTE>& v)
{
	RCF::RcfSession& ConnectSession = RCF::getCurrentRcfSession();
	SagaServiceSession& SagaSession = ConnectSession.getSessionObject<SagaServiceSession>(true);

	auto VecDecode = SRC4Decode(v, SagaSession.GetRC4Key());
	auto lPackRead = std::make_shared<PackReader>(VecDecode);

	//MAC 地址与时间戳
	std::string& Mac = SagaSession.GetMAC();
	ULONG& TimeStamp = SagaSession.GetTimeStamp();
	auto VTimeStamp  = lPackRead->ReadInt();
	auto VMac        = lPackRead->ReadStr();
	std::string Result;
	BOOL bRet  = FALSE;


	if (Mac.empty())
	{
		//第一次.
		Mac       = VMac;
		TimeStamp = VTimeStamp;
		bRet      = true;
		Result    = xorstr_("init succeed");
	}
	else
	{
		if (Mac != VMac)
		{
			//MAC 不匹配
			Result = xorstr_("Mac Mismatch");
			ACEDebugFileLog("[{}] SrcMac :{} CurMac:{}", __FUNCTION__, Mac, VMac);
		}
		else
		{
			//判断时间差 ＞3分钟 = 掉线
			auto ValueDiff = abs(int(VTimeStamp - TimeStamp));
			// 30 秒一次 - 
			if (ValueDiff > 30 * 3  )
			{
				Result = xorstr_("client timeout");

				ACEDebugFileLog("[{}] ", __FUNCTION__, Result);

			}
			else
			{
				TimeStamp = VTimeStamp;
				bRet = true;
				Result = xorstr_("succeed");
			}
			ACEDebugFileLog("[{}] ValueDiff :{} ", __FUNCTION__,  ValueDiff);

		}

	}

	std::vector<BYTE> VecResult(1024 + Result.length(), 0);

	auto lPackWrite = std::make_shared<PackWriter>(VecResult);

	lPackWrite->WriteByte(bRet);

	lPackWrite->WriteString(Result);

	lPackWrite->WriteInt(VTimeStamp);

	return SRC4Encrypt(VecResult, SagaSession.GetRC4Key());
}


std::vector<BYTE> SagaService::RsaPriEncrypt(std::string& Text, std::string& PriKey)
{
	VMProtectBegin(__FUNCTION__);

	std::vector<BYTE> CipherText;

	BIO* keybio = BIO_new_mem_buf((unsigned char*)PriKey.data(), -1);
	RSA* rsa = RSA_new();
	rsa = PEM_read_bio_RSAPrivateKey(keybio, &rsa, NULL, NULL);
	if (!rsa)
	{	
		char err_msg[1024] = { 0 };
		ERR_error_string(ERR_get_error(), err_msg); 
		BIO_free_all(keybio);
		return CipherText;
	}

	int key_len = RSA_size(rsa);
	int block_len = key_len - 11;    // 因为填充方式为RSA_PKCS1_PADDING, 所以要在key_len基础上减去11

	std::vector<BYTE> VecText(key_len + 1);

	int ret = 0;
	int pos = 0;
	std::string sub_str;
	
	while (pos < Text.length())
	{
		sub_str = Text.substr(pos, block_len);
		memset(VecText.data(), 0, key_len + 1);
		ret = RSA_private_encrypt(sub_str.length(), (const unsigned char*)sub_str.c_str(), (unsigned char*)VecText.data(), rsa, RSA_PKCS1_PADDING);
		if (ret >= 0)
		{
			int Pos = CipherText.size();
			CipherText.resize(Pos + ret,0);
			memcpy((CipherText.data()+ Pos), VecText.data(), ret);
		}
		pos += block_len;
	}

	BIO_free_all(keybio);
	RSA_free(rsa);

	VMProtectEnd();
	return CipherText;
}

std::vector<BYTE> SagaService::RsaPriDecrypt(std::vector<BYTE>& VecText, std::string& PriKey)
{
	VMProtectBegin(__FUNCTION__);

	std::vector<BYTE> decrypt_text;
	RSA* rsa = RSA_new();
	BIO* keybio = nullptr;
	keybio = BIO_new_mem_buf((unsigned char*)PriKey.c_str(), -1);

	rsa = PEM_read_bio_RSAPrivateKey(keybio, &rsa, NULL, NULL);
	if (rsa == nullptr)
	{
		char err_msg[1024] = { 0 };
		ERR_error_string(ERR_get_error(), err_msg); // 格式：error:errId:库:函数:原因
		BIO_free_all(keybio);
		return decrypt_text;
	}

	// 获取RSA单次处理的最大长度
	int key_len = RSA_size(rsa);
	std::vector<BYTE> sub_text(key_len);

	int ret = 0;
	int pos = 0;

	// 对密文进行分段解密
	while (pos < VecText.size())
	{
		memset(sub_text.data(), 0, key_len);
		ret = RSA_private_decrypt(key_len, (const unsigned char*)VecText.data() + pos, (unsigned char*)sub_text.data(), rsa, RSA_PKCS1_PADDING);
		if (ret >= 0)
		{
			int Pos = decrypt_text.size();
			decrypt_text.resize(Pos + ret, 0);
			memcpy(decrypt_text.data() + pos, sub_text.data(), ret);
		}
		pos += key_len;
	}
	BIO_free_all(keybio);
	RSA_free(rsa);
	VMProtectEnd();


	return decrypt_text;
}

std::vector<BYTE> SagaService::RsaPubDecrypt(std::vector<BYTE>& VecText, std::string& PubKey)
{
	VMProtectBegin(__FUNCTION__);

	std::vector<BYTE> decrypt_text;
	RSA* rsa    = RSA_new();
	BIO* keybio = nullptr;
	keybio      = BIO_new_mem_buf((unsigned char*)PubKey.c_str(), -1);

	rsa = /*PEM_read_bio_RSAPrivateKey*/PEM_read_bio_RSA_PUBKEY(keybio, &rsa, NULL, NULL);
	if (rsa == nullptr) 
	{
		char err_msg[1024] = { 0 };
		ERR_error_string(ERR_get_error(), err_msg); // 格式：error:errId:库:函数:原因
		BIO_free_all(keybio);
		return decrypt_text;
	}

	// 获取RSA单次处理的最大长度
	int key_len = RSA_size(rsa);
	std::vector<BYTE> sub_text(key_len);

	int ret = 0;
	int pos = 0;

	// 对密文进行分段解密
	while (pos < VecText.size())
	{
		memset(sub_text.data(),0, key_len);
		ret = /*RSA_private_decrypt*/RSA_public_decrypt(key_len, (const unsigned char*)VecText.data() + pos, (unsigned char*)sub_text.data(), rsa, RSA_PKCS1_PADDING);
		if (ret >= 0) 
		{
			int Pos = decrypt_text.size();
			decrypt_text.resize(Pos + ret, 0);
			memcpy(decrypt_text.data() + pos, sub_text.data(), ret);
		}
		pos += key_len;
	}
	BIO_free_all(keybio);
	RSA_free(rsa);
	VMProtectEnd();


	return decrypt_text;
}


std::vector<BYTE> SagaService::RsaPubEncrypt(std::string& Text, std::string& PubKey)
{
	VMProtectBegin(__FUNCTION__);

	std::vector<BYTE> CipherText;

	BIO* keybio = BIO_new_mem_buf((unsigned char*)PubKey.data(), -1);
	RSA* rsa = RSA_new();
	rsa = PEM_read_bio_RSA_PUBKEY(keybio, &rsa, NULL, NULL);
	if (!rsa)
	{
		char err_msg[1024] = { 0 };
		ERR_error_string(ERR_get_error(), err_msg);
		BIO_free_all(keybio);
		return CipherText;
	}

	int key_len = RSA_size(rsa);
	int block_len = key_len - 11;   

	std::vector<BYTE> VecText(key_len + 1);

	int ret = 0;
	int pos = 0;
	std::string sub_str;

	while (pos < Text.length())
	{
		sub_str = Text.substr(pos, block_len);
		memset(VecText.data(), 0, key_len + 1);
		ret = RSA_public_encrypt(sub_str.length(), (const unsigned char*)sub_str.c_str(), (unsigned char*)VecText.data(), rsa, RSA_PKCS1_PADDING);
		if (ret >= 0)
		{
			int Pos = CipherText.size();
			CipherText.resize(Pos + ret, 0);
			memcpy((CipherText.data() + Pos), VecText.data(), ret);
		}
		pos += block_len;
	}

	BIO_free_all(keybio);
	RSA_free(rsa);

	VMProtectEnd();
	return CipherText;
}

std::vector<BYTE> SagaService::Base64Decode(std::string Text)
{
	BIO* bio_hnd_meth    = nullptr;
	bio_hnd_meth = BIO_new(BIO_f_base64());
	BIO_set_flags(bio_hnd_meth, BIO_FLAGS_BASE64_NO_NL);
	std::vector<BYTE> VecResult(Text.length(),0);
	//根据编码了的数据创建一个bio内存对象
	BIO* bmem = BIO_new_mem_buf(Text.data(), Text.length());
	BIO_push(bio_hnd_meth, bmem);
	BIO_flush(bio_hnd_meth);
	//解码数据
	int len = BIO_read(bio_hnd_meth, VecResult.data(), VecResult.size());

	//释放资源
	BIO_free(bio_hnd_meth);
	BIO_free_all(bmem);
	VecResult.resize(len);
	
	return VecResult;

}

std::string SagaService::Base64Encode(std::vector<BYTE> Src)
{
	BIO* bio_hnd_mem     = nullptr;
	BIO* bio_hnd_meth    = nullptr;
	BUF_MEM* buf_mem_ptr = nullptr;


	bio_hnd_meth = BIO_new(BIO_f_base64());
	BIO_set_flags(bio_hnd_meth, BIO_FLAGS_BASE64_NO_NL);
	bio_hnd_mem = BIO_new(BIO_s_mem());
	bio_hnd_meth = BIO_push(bio_hnd_meth, bio_hnd_mem);

	BIO_write(bio_hnd_meth, Src.data(), Src.size());
	BIO_flush(bio_hnd_meth);
	BIO_get_mem_ptr(bio_hnd_meth, &buf_mem_ptr);
	BIO_set_close(bio_hnd_meth, BIO_NOCLOSE);

	std::vector<BYTE> VecDst(buf_mem_ptr->length);

	memcpy(VecDst.data(), buf_mem_ptr->data, buf_mem_ptr->length);

	BUF_MEM_free(buf_mem_ptr);
	BIO_free(bio_hnd_meth);
	BIO_free(bio_hnd_mem);

	return std::string(VecDst.begin(), VecDst.end());
}

