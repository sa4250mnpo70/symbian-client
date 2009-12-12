/*
============================================================================
 Name        : BuddycloudEditChannelList.h
 Author      : Ross Savage
 Copyright   : Buddycloud 2009
 Description : Declares Edit Channel List
============================================================================
*/

#ifndef BUDDYCLOUDEDITCHANNELLIST_H_
#define BUDDYCLOUDEDITCHANNELLIST_H_

// INCLUDES
#include <coecntrl.h>
#include <aknsettingitemlist.h>
#include "BuddycloudEditChannelList.h"
#include "BuddycloudLogic.h"
#include "XmppInterfaces.h"

// CLASS DECLARATION
class CBuddycloudEditChannelList : public CAknSettingItemList, MXmppStanzaObserver {
	public: // Constructors and destructor
		CBuddycloudEditChannelList();
		void ConstructL(const TRect& aRect, CBuddycloudLogic* aBuddycloudLogic, TInt aItemId);
		~CBuddycloudEditChannelList();
		
	private:
		void SetTitleL(TInt aResourceId);
		void ValidateChannelId();

	public:
		void EditCurrentItemL();
		void LoadChannelDataL();
		TInt SaveChannelDataL();

	public:
		void DynInitMenuPaneL(TInt aResourceId, CEikMenuPane* aMenuPane);
		
	public: // From CCoeControl
		void GetHelpContext(TCoeHelpContext& aContext) const;

	public: // From CAknSettingItemList
		void EditItemL(TInt aIndex, TBool aCalledFromMenu);

	private:
		CAknSettingItem* CreateSettingItemL (TInt aSettingId);

	public: // From MXmppStanzaObserver
		void XmppStanzaAcknowledgedL(const TDesC8& aStanza, const TDesC8& aId);

	private: // Data
		CBuddycloudLogic* iBuddycloudLogic;	
		MXmppWriteInterface* iXmppInterface;

		CFollowingChannelItem* iChannelItem;
		TBool iChannelSaveAllowed;
		
		TInt iTitleResourceId;
		
		TBuf<64> iTitle;
		TBuf<64> iId;
		TBuf<256> iDescription;
		TInt iAccess;
};

#endif

// End of File