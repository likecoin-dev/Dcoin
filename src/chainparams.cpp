// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2012 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "assert.h"

#include "chainparams.h"
#include "core.h"
#include "protocol.h"
#include "util.h"

//
// Main network                                                                 Основная сеть
//

unsigned int pnSeed[] =
{
    0x0289a8c0, 0x1689a8c0  // должно быть правильно(192.168.137.2, 192.168.137.22), но для старта нужно два и более сконнектившихся клиента при добавленных addnode вручную
};

class CMainParams : public CChainParams {
public:
    CMainParams() {
        // The message start string is designed to be unlikely to occur in normal data. Сообщение начала строки - это вряд ли произойдет в обычных данных.
        // The characters are rarely used upper ASCII, not valid as UTF-8, and produce  Символы редко используют верхний ASCII, не действует как UTF-8, и производят
        // a large 4-byte int at any alignment.                                         большое 4-байт int в любое выравнивание.
        pchMessageStart[0] = 0xf9;
        pchMessageStart[1] = 0xbe;
        pchMessageStart[2] = 0xb4;
        pchMessageStart[3] = 0xd9;
        vAlertPubKey = ParseHex("04fc9702847840aaf195de8442ebecedf5b095cdbb9bc716bda9110971b28a49e0ead8564ff0db22209e0374782c093bb899692d524e9d6a6956e7c5ecbcd68284");
        nDefaultPort = 28333;
        nRPCPort = 28332;
        bnProofOfWorkLimit = CBigNum(~uint256(0) >> 1);
        nSubsidyHalvingInterval = 210;

        // Build the genesis block. Note that the output of the genesis coinbase cannot
        // be spent as it did not originally exist in the database.
        //                  Построение начального блока. Заметьте, что вывод генезиса coinbase не может
        //                  быть потрачен так как его первоначально не существует в базе данных.
        //
        // CBlock(hash=000000000019d6, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=4a5e1e, nTime=1231006505, nBits=1d00ffff, nNonce=2083236893, vtx=1)
        //   CTransaction(hash=4a5e1e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
        //     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d0104455468652054696d65732030332f4a616e2f32303039204368616e63656c6c6f72206f6e206272696e6b206f66207365636f6e64206261696c6f757420666f722062616e6b73)
        //     CTxOut(nValue=50.00000000, scriptPubKey=0x5F1DF16B2B704C8A578D0B)
        //   vMerkleTree: 4a5e1e
        const char* pszTimestamp = "The Times 03/Jan/2009 Chancellor on brink of second bailout for banks";
        CTransaction txNew;
        txNew.vin.resize(1);
        txNew.vout.resize(1);
        txNew.vin[0].scriptSig = CScript() << 486604799 << CBigNum(4) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        txNew.vout[0].nValue = 50 * COIN;
//        txNew.vout[0].Dvalue = 11 * COIN;       ////////// новое //////////
        txNew.vout[0].scriptPubKey = CScript() << ParseHex("04678afdb0fe5548271967f1a67130b7105cd6a828e03909a67962e0ea1f61deb649f6bc3f4cef38c4f35504e51ec112de5c384df7ba0b8d578a4c702b6bf11d5f") << OP_CHECKSIG;
        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock = 0;
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion = 1;
        genesis.nTime    = 1231006505;
        genesis.nBits    = 0x207fffff;
//        genesis.nBits    = 0x1d00ffff;
        genesis.nNonce   = 2083236893;
        genesis.dBase0   = 20000000;        ////////// новое //////////
        genesis.dBase1   = 20000000;        ////////// новое //////////
        genesis.coinD0   = 20000000;        ////////// новое //////////
        genesis.coinD1   = 20000000;        ////////// новое //////////

        hashGenesisBlock = genesis.GetHash();
        printf("hashGenesisBlock  %s\n\n",hashGenesisBlock.ToString().c_str());
//        genesis.print();                                  // tttttttttttttt

//        assert(hashGenesisBlock == uint256("0x68cee22f4538bffc46b9372798f641af44f98539b1cf646af0f8ff3a196163c1"));
//        assert(hashGenesisBlock == uint256("0x000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"));
        assert(genesis.hashMerkleRoot == uint256("0x4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"));

//        vSeeds.push_back(CDNSSeedData("bitcoin.sipa.be", "seed.bitcoin.sipa.be"));
//        vSeeds.push_back(CDNSSeedData("bluematt.me", "dnsseed.bluematt.me"));
//        vSeeds.push_back(CDNSSeedData("dashjr.org", "dnsseed.bitcoin.dashjr.org"));
//        vSeeds.push_back(CDNSSeedData("xf2.org", "bitseed.xf2.org"));
        vSeeds.clear();

        base58Prefixes[PUBKEY_ADDRESS] = 30;        // https://en.bitcoin.it/wiki/List_of_address_prefixes
        base58Prefixes[SCRIPT_ADDRESS] = 5;
        base58Prefixes[SECRET_KEY] = 128;

        // Convert the pnSeeds array into usable address objects.                       Преобразование массива pnSeeds в используемые адреса объектов
        for (unsigned int i = 0; i < ARRAYLEN(pnSeed); i++)
        {
            // It'll only connect to one or two seed nodes because once it connects,    Оно будет подключиться только к одной или двум сидам узла, потому что, как только
            // it'll get a pile of addresses with newer timestamps.                     он подключается, он будет получать кучу адресов с новыми временными метками.
            // Seed nodes are given a random 'last seen time' of between one and two    Сиды узлов дают случайный 'время последнего появления' от одного до двух
            // weeks ago.                                                               недель назад.
            const int64 nOneWeek = 7*24*60*60;
            struct in_addr ip;
            memcpy(&ip, &pnSeed[i], sizeof(ip));
            CAddress addr(CService(ip, GetDefaultPort()));
            addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
            vFixedSeeds.push_back(addr);
        }
    }

    virtual const CBlock& GenesisBlock() const { return genesis; }
    virtual Network NetworkID() const { return CChainParams::MAIN; }

    virtual const vector<CAddress>& FixedSeeds() const {
        return vFixedSeeds;
    }
protected:
    CBlock genesis;
    vector<CAddress> vFixedSeeds;
};
static CMainParams mainParams;


//
// Testnet (v3)
//
class CTestNetParams : public CMainParams {
public:
    CTestNetParams() {
/*        // The message start string is designed to be unlikely to occur in normal data. Сообщение начала строки - это вряд ли произойдет в обычных данных.
        // The characters are rarely used upper ASCII, not valid as UTF-8, and produce  Символы редко используют верхний ASCII, не действует как UTF-8, и производят
        // a large 4-byte int at any alignment.                                         большое 4-байт int в любое выравнивание.
        pchMessageStart[0] = 0x0b;
        pchMessageStart[1] = 0x11;
        pchMessageStart[2] = 0x09;
        pchMessageStart[3] = 0x07;
        vAlertPubKey = ParseHex("04302390343f91cc401d56d68b123028bf52e5fca1939df127f63c6467cdf9c8e2c14b61104cf817d0b780da337893ecc4aaff1309e536162dabbdb45200ca2b0a");
        nDefaultPort = 38333;
        nRPCPort = 38332;
        strDataDir = "testnet3";

        // Modify the testnet genesis block so the timestamp is valid for a later start.    Измените блок генезис testnet так чтобы временная метка была действительна для позднего старта.
        genesis.nTime = 1296688602;
        genesis.nNonce = 414098458;
        genesis.dBase0    = 50 * COIN;      ////////// новое //////////
        genesis.dBase1    = 50 * COIN;      ////////// новое //////////
        genesis.coinD0   = 50 * COIN;      ////////// новое //////////
        genesis.coinD1   = 50 * COIN;      ////////// новое //////////
        hashGenesisBlock = genesis.GetHash();
        assert(hashGenesisBlock == uint256("000000000933ea01ad0ee984209779baaec3ced90fa3f408719526f8d77f4943"));

        vFixedSeeds.clear();
//        vSeeds.clear();
//        vSeeds.push_back(CDNSSeedData("bitcoin.petertodd.org", "testnet-seed.bitcoin.petertodd.org"));
//        vSeeds.push_back(CDNSSeedData("bluematt.me", "testnet-seed.bluematt.me"));

        base58Prefixes[PUBKEY_ADDRESS] = 111;
        base58Prefixes[SCRIPT_ADDRESS] = 196;
        base58Prefixes[SECRET_KEY] = 239;
*/
        pchMessageStart[0] = 0xfa;
        pchMessageStart[1] = 0xbf;
        pchMessageStart[2] = 0xb5;
        pchMessageStart[3] = 0xda;
        nSubsidyHalvingInterval = 150;
        bnProofOfWorkLimit = CBigNum(~uint256(0) >> 1);
        genesis.nTime = 1296688602;
        genesis.nBits = 0x207fffff;
        genesis.nNonce = 2;
        genesis.dBase0   = 55;      ////////// новое //////////
        genesis.dBase1   = 55;      ////////// новое //////////
        genesis.coinD0   = 55;      ////////// новое //////////
        genesis.coinD1   = 55;      ////////// новое //////////
        hashGenesisBlock = genesis.GetHash();
        nDefaultPort = 38444;
        strDataDir = "testnet3";
//        assert(hashGenesisBlock == uint256("0x0f9188f13cb7b2c71f2a335e3a4fc328bf5beb436012afca590b1a11466e2206"));

        vSeeds.clear();  // Regtest mode doesn't have any DNS seeds.                    Regtest режим не имеет любых DNS сидов.

        base58Prefixes[PUBKEY_ADDRESS] = 30;
        base58Prefixes[SCRIPT_ADDRESS] = 5;
        base58Prefixes[SECRET_KEY] = 128;
    }
    virtual Network NetworkID() const { return CChainParams::TESTNET; }
};
static CTestNetParams testNetParams;


//
// Regression test (Регрессионное тестирование)
//
class CRegTestParams : public CTestNetParams {
public:
    CRegTestParams() {
        pchMessageStart[0] = 0xfa;
        pchMessageStart[1] = 0xbf;
        pchMessageStart[2] = 0xb5;
        pchMessageStart[3] = 0xda;
        nSubsidyHalvingInterval = 150;
        bnProofOfWorkLimit = CBigNum(~uint256(0) >> 1);
        genesis.nTime = 1296688602;
        genesis.nBits = 0x207fffff;
        genesis.nNonce = 2;
        genesis.dBase0   = 55;      ////////// новое //////////
        genesis.dBase1   = 55;      ////////// новое //////////
        genesis.coinD0   = 55;      ////////// новое //////////
        genesis.coinD1   = 55;      ////////// новое //////////
        hashGenesisBlock = genesis.GetHash();
        nDefaultPort = 38444;
        strDataDir = "regtest";
//        assert(hashGenesisBlock == uint256("0x0f9188f13cb7b2c71f2a335e3a4fc328bf5beb436012afca590b1a11466e2206"));

        vSeeds.clear();  // Regtest mode doesn't have any DNS seeds.                    Regtest режим не имеет любых DNS сидов.

        base58Prefixes[PUBKEY_ADDRESS] = 30;
        base58Prefixes[SCRIPT_ADDRESS] = 5;
        base58Prefixes[SECRET_KEY] = 128;
    }

    virtual bool RequireRPCPassword() const { return false; }
    virtual Network NetworkID() const { return CChainParams::REGTEST; }
};
static CRegTestParams regTestParams;

static CChainParams *pCurrentParams = &mainParams;

const CChainParams &Params() {
    return *pCurrentParams;
}

void SelectParams(CChainParams::Network network) {
    switch (network) {
        case CChainParams::MAIN:
            pCurrentParams = &mainParams;
            break;
        case CChainParams::TESTNET:
            pCurrentParams = &testNetParams;
            break;
        case CChainParams::REGTEST:
            pCurrentParams = &regTestParams;
            break;
        default:
            assert(false && "Unimplemented network");
            return;
    }
}

bool SelectParamsFromCommandLine() {
    bool fRegTest = GetBoolArg("-regtest", false);
    bool fTestNet = GetBoolArg("-testnet", false);

    if (fTestNet && fRegTest) {
        return false;
    }

    if (fRegTest) {
        SelectParams(CChainParams::REGTEST);
    } else if (fTestNet) {
        SelectParams(CChainParams::TESTNET);
    } else {
        SelectParams(CChainParams::MAIN);
    }
    return true;
}
