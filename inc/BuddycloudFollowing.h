/*
============================================================================
 Name        : 	BuddycloudFollowing.h
 Author      : 	Ross Savage
 Copyright   : 	Buddycloud 2008
 Description : 	Storage of Buddycloud following based data
 History     : 	1.0

				1.0	Initial Development
============================================================================
*/

#ifndef BUDDYCLOUDFOLLOWING_H_
#define BUDDYCLOUDFOLLOWING_H_

#include <e32base.h>
#include "BuddycloudList.h"
#include "DiscussionManager.h"
#include "GeolocData.h"
#include "XmppConstants.h"

const TInt KEditingNewChannelId = -9999;

/*
----------------------------------------------------------------------------
--
-- Enumerations
--
----------------------------------------------------------------------------
*/

enum TFollowingItemType {
	EItemNotice = 2, 
	EItemContact = 4, 
	EItemRoster = 8, 
	EItemChannel = 16,
	// Private: do not use
	EItemAll = 30
};

enum TIdType {
	EIdRoster, EIdChannel
};

enum TPresenceState {
	EPresenceOffline, EPresenceOnline
};

enum TNoticeResponse {
	ENoticeAccept, ENoticeAcceptAndFollow, ENoticeDecline
};

enum TGeolocItemType {
	EGeolocItemPrevious, EGeolocItemCurrent, EGeolocItemFuture, EGeolocItemBroad
};

/*
----------------------------------------------------------------------------
--
-- CFollowingItem
--
----------------------------------------------------------------------------
*/

class CFollowingItem : public CBuddycloudListItem {
	public:
		static CFollowingItem* NewL();
		static CFollowingItem* NewLC();
		~CFollowingItem();
		
	protected:
		CFollowingItem();
		void ConstructL();

	public:
		TFollowingItemType GetItemType();
		void SetItemType(TFollowingItemType aItemType);

		TDesC& GetId(TIdType aType = EIdChannel);
		void SetIdL(const TDesC& aId, TIdType aType = EIdChannel);
		
		TIdType GetIdType();

	protected:
		TFollowingItemType iItemType;

		HBufC* iId;
		TIdType iIdType;
};

/*
----------------------------------------------------------------------------
--
-- CFollowingContactItem
--
----------------------------------------------------------------------------
*/

class CFollowingContactItem : public CFollowingItem {
	public:
		static CFollowingContactItem* NewL();
		static CFollowingContactItem* NewLC();
		
	protected:
		CFollowingContactItem();

	public:
		TInt GetAddressbookId();
		void SetAddressbookId(TInt aAddressbookId);

	protected:
		TInt iAddressbookId;
};

/*
----------------------------------------------------------------------------
--
-- CFollowingChannelItem
--
----------------------------------------------------------------------------
*/

class CFollowingChannelItem : public CFollowingContactItem {
	public:
		static CFollowingChannelItem* NewL();
		static CFollowingChannelItem* NewLC();
	
	protected:
		CFollowingChannelItem();

// New		
	public:
		TXmppPubsubAccessModel GetAccessModel();
		void SetAccessModel(TXmppPubsubAccessModel aAccessModel);
		
		TXmppPubsubSubscription GetPubsubSubscription();
		void SetPubsubSubscription(TXmppPubsubSubscription aPubsubSubscription);
		
		TXmppPubsubAffiliation GetPubsubAffiliation();
		void SetPubsubAffiliation(TXmppPubsubAffiliation aPubsubAffiliation);
		
	public:
		TInt GetRank();
		TInt GetRankShift();
		void SetRank(TInt aRank, TInt aShift = 0);
		
	public:
		void SetUnreadData(MDiscussionUnreadData* aUnreadData, TIdType aType = EIdChannel);
		
	public:
		TInt GetUnread(TIdType aType = EIdChannel);
		TInt GetReplies();
		
	protected:
		MDiscussionUnreadData* iChannelUnreadData;
		
		TXmppPubsubAccessModel iAccessModel;
		TXmppPubsubSubscription iPubsubSubscription;
		TXmppPubsubAffiliation iPubsubAffiliation;
		
		TInt iRank;
		TInt iRankShift;
};

/*
----------------------------------------------------------------------------
--
-- CFollowingRosterItem
--
----------------------------------------------------------------------------
*/

class CFollowingRosterItem : public CFollowingChannelItem {
	public:
		static CFollowingRosterItem* NewL();
		static CFollowingRosterItem* NewLC();
		~CFollowingRosterItem();
	
	protected:
		CFollowingRosterItem();
		void ConstructL();
		
	public:
		TBool OwnItem();
		void SetOwnItem(TBool aIsOwn);

	public:
		TDesC& GetId(TIdType aType = EIdRoster);
		void SetIdL(TDesC& aId, TIdType aType = EIdRoster);
				
	public:
		TPresenceState GetPresence();
		void SetPresence(TPresenceState aPresence);

		TPresenceSubscription GetSubscription();
		void SetSubscription(TPresenceSubscription aSubscription);

	public:
		TBool PubsubCollected();
		void SetPubsubCollected(TBool aCollected);

		CGeolocData* GetGeolocItem(TGeolocItemType aGeolocItem);
		void SetGeolocItemL(TGeolocItemType aGeolocItem, CGeolocData* aGeoloc);
		
	public:
		void SetUnreadData(MDiscussionUnreadData* aUnreadData, TIdType aType = EIdRoster);
		
	public:
		TInt GetUnread(TIdType aType = EIdRoster);

	protected:
		MDiscussionUnreadData* iRosterUnreadData;
		
		HBufC* iJid;

		TBool iOwnItem;
		TBool iPubsubCollected;		
		
		TPresenceSubscription iSubscription;
		TPresenceState iPresence;

		RPointerArray<CGeolocData> iGeolocs;
};

#endif /*BUDDYCLOUDFOLLOWING_H_*/