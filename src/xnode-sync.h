// Copyright (c) 2014-2017 The Dash Core developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
#ifndef XNODE_SYNC_H
#define XNODE_SYNC_H

#include "chain.h"
#include "net.h"

#include <univalue.h>

class CXnodeSync;

static const int XNODE_SYNC_FAILED          = -1;
static const int XNODE_SYNC_INITIAL         = 0;
static const int XNODE_SYNC_LIST            = 2;
static const int XNODE_SYNC_MNW             = 3;
static const int XNODE_SYNC_FINISHED        = 999;
static const int XNODE_SYNC_TICK_SECONDS    = 6;
static const int XNODE_SYNC_TIMEOUT_SECONDS = 30; // our blocks are 2.5 minutes so 30 seconds should be fine
static const int XNODE_SYNC_ENOUGH_PEERS    = 6;
extern CXnodeSync xnodeSync;

//
// CXnodeSync : Sync xnode assets in stages
//

class CXnodeSync
{
private:
    // Keep track of current asset
    int nRequestedXnodeAssets;
    // Count peers we've requested the asset from
    int nRequestedXnodeAttempt;

    // Time when current xnode asset sync started
    int64_t nTimeAssetSyncStarted;

    // Last time when we received some xnode asset ...
    int64_t nTimeLastXnodeList;
    int64_t nTimeLastPaymentVote;
    int64_t nTimeLastFailure;

    // How many times we failed
    int nCountFailures;

    // Keep track of current block index
    const CBlockIndex *pCurrentBlockIndex;

    bool CheckNodeHeight(CNode* pnode, bool fDisconnectStuckNodes = false);
    void Fail();
    void ClearFulfilledRequests();

public:
    CXnodeSync() { Reset(); }

    void AddedXnodeList() { nTimeLastXnodeList = GetTime(); }
    void AddedPaymentVote() { nTimeLastPaymentVote = GetTime(); }

    bool IsFailed() { return nRequestedXnodeAssets == XNODE_SYNC_FAILED; }
    bool IsBlockchainSynced(bool fBlockAccepted = false);
    bool IsXnodeListSynced() { return nRequestedXnodeAssets > XNODE_SYNC_LIST; }
    bool IsWinnersListSynced() { return nRequestedXnodeAssets > XNODE_SYNC_MNW; }
    bool IsSynced() { return nRequestedXnodeAssets == XNODE_SYNC_FINISHED; }

    int GetAssetID() { return nRequestedXnodeAssets; }
    int GetAttempt() { return nRequestedXnodeAttempt; }
    std::string GetAssetName();
    std::string GetSyncStatus();

    void Reset();
    void SwitchToNextAsset();

    void ProcessMessage(CNode* pfrom, std::string& strCommand, CDataStream& vRecv);
    void ProcessTick();

    void UpdatedBlockTip(const CBlockIndex *pindex);
};

#endif
