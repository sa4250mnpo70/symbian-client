/*
============================================================================
 Name        : 	BuddycloudExplorer.h
 Author      : 	Ross Savage
 Copyright   : 	Buddycloud 2009
 Description : 	Storage types for Buddycloud explorer data
 History     : 	1.0

				1.0	Initial Development
============================================================================
*/

#ifndef BUDDYCLOUDEXPLORER_H_
#define BUDDYCLOUDEXPLORER_H_

#include <e32base.h>
#include "BuddycloudList.h"
#include "GeolocData.h"

/*
----------------------------------------------------------------------------
--
-- Enumerations
--
----------------------------------------------------------------------------
*/

enum TExplorerChannelRole {
	EChannelFollower, EChannelModerator, EChannelProducer, EChannelAll
};

enum TExplorerItemType {
	EExplorerItemUnknown, EExplorerItemDirectory, EExplorerItemPlace, 
	EExplorerItemPerson, EExplorerItemChannel, EExplorerItemLink, EExplorerItemEvent
};

enum TExplorerState {
	EExplorerIdle, EExplorerRequesting
};

/*
----------------------------------------------------------------------------
--
-- TExplorerQuery
--
----------------------------------------------------------------------------
*/

const TInt KExplorerMaxTitleSize  = 128;
const TInt KExplorerMaxStanzaSize = 512;
	
class TExplorerQuery {
	public:
		TBuf<KExplorerMaxTitleSize> iTitle;
		TBuf8<KExplorerMaxStanzaSize> iStanza;
};

typedef TPckg<TExplorerQuery> TExplorerQueryPckg;

/*
----------------------------------------------------------------------------
--
-- CExplorerStanzaBuilder
--
----------------------------------------------------------------------------
*/

class CExplorerStanzaBuilder {
	public:
		// Nearby stanzas
		static TExplorerQuery BuildNearbyXmppStanza(TExplorerItemType aType, const TDesC8& aReferenceId, TInt aOptionsLimit = 10);
		static TExplorerQuery BuildNearbyXmppStanza(TReal aPointLatitude, TReal aPointLongitude, TInt aOptionsLimit = 10);

		// Channels stanzas
		static TExplorerQuery BuildChannelsXmppStanza(const TDesC8& aSubject, TExplorerChannelRole aRole, TInt aOptionsLimit = 0);

		// Place stanzas
		static TExplorerQuery BuildPlaceVisitorsXmppStanza(const TDesC8& aNode, TInt aPlaceId);
};

/*
----------------------------------------------------------------------------
--
-- CExplorerResultItem
--
----------------------------------------------------------------------------
*/

class CExplorerResultItem : public CBuddycloudListItem {
	public:
		static CExplorerResultItem* NewL();
		static CExplorerResultItem* NewLC();
		~CExplorerResultItem();
		
	private:
		void ConstructL();
	
	public:
		TExplorerItemType GetResultType();
		void SetResultType(TExplorerItemType aResultType);
		
		TInt GetOverlayId();
		void SetOverlayId(TInt aOverlayId);
		
		TInt GetRank();
		void SetRank(TInt aRank);
		
	public:
		TDesC& GetId();
		void SetIdL(const TDesC& aId);
		
	public:
		CGeolocData* GetGeoloc();
		void SetGeolocL(CGeolocData* aGeoloc);
		
		void UpdateFromGeolocL();
	
	protected:
		TExplorerItemType iResultType;
		
		TInt iOverlayId;
		TInt iRank;
		
		HBufC* iId;
		
		CGeolocData* iGeoloc;
};

/*
----------------------------------------------------------------------------
--
-- CExplorerQueryLevel
--
----------------------------------------------------------------------------
*/

class CExplorerQueryLevel : public CBase {
	public:
		static CExplorerQueryLevel* NewL();
		static CExplorerQueryLevel* NewLC();
		~CExplorerQueryLevel();
	
	private:
		void ConstructL();
		
	public:
		TDesC& GetQueryTitle();
		void SetQueryTitleL(const TDesC& aTitle);
		
	public:
		TDesC8& GetQueriedStanza();
		void SetQueriedStanzaL(const TDesC8& aStanza);
		
	public:
		void ClearResultItems();
	
	protected:
		HBufC* iQueryTitle;
		HBufC8* iQueriedStanza;
				
	public:
		TInt iSelectedResultItem;

		RPointerArray<CExplorerResultItem> iResultItems;
};

#endif /* BUDDYCLOUDEXPLORER_H_ */