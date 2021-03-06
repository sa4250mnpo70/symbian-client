/*
============================================================================
 Name        : BuddycloudEditPlaceList.h
 Author      : Ross Savage
 Copyright   : 2008 Buddycloud
 Description : Declares Edit Place List
============================================================================
*/

#include <akntitle.h>
#include <akntextsettingpage.h>
#include <eikspane.h>
#include <Buddycloud_lang.rsg>
#include "Buddycloud.hrh"
#include "BuddycloudEditPlaceList.h"

CBuddycloudEditPlaceList::CBuddycloudEditPlaceList() : CAknSettingItemList() {
}

void CBuddycloudEditPlaceList::ConstructL(const TRect& aRect, CBuddycloudLogic* aBuddycloudLogic) {
	iBuddycloudLogic = aBuddycloudLogic;
	iBuddycloudLogic->AddNotificationObserver(this);

	// Load place data
	GetEditedPlace();
	
	iTitleResourceId = R_LOCALIZED_STRING_NEWPLACE_TITLE;

	if(iEditingPlace && iEditingPlace->GetItemId() > 0) {		
		iTitleResourceId = R_LOCALIZED_STRING_EDITPLACE_TITLE;
	}

	SetTitleL(iTitleResourceId);
	SetRect(aRect);
}

CBuddycloudEditPlaceList::~CBuddycloudEditPlaceList() {
	iBuddycloudLogic->RemoveNotificationObserver(this);
}

void CBuddycloudEditPlaceList::SetTitleL(TInt aResourceId) {
	CAknTitlePane* aTitlePane = static_cast<CAknTitlePane*>(iEikonEnv->AppUiFactory()->StatusPane()->ControlL(TUid::Uid(EEikStatusPaneUidTitle)));
	HBufC* aTitle = iEikonEnv->AllocReadResourceLC(aResourceId);
	aTitlePane->SetTextL(*aTitle);
	CleanupStack::PopAndDestroy(); // aTitle
}

void CBuddycloudEditPlaceList::GetEditedPlace() {
	iEditingPlace = static_cast <CBuddycloudExtendedPlace*> (iBuddycloudLogic->GetPlaceStore()->GetEditedPlace());

	if(iEditingPlace) {
		iName = iEditingPlace->GetGeoloc()->GetString(EGeolocText);
		iStreet = iEditingPlace->GetGeoloc()->GetString(EGeolocStreet);
		iArea = iEditingPlace->GetGeoloc()->GetString(EGeolocArea);
		iCity = iEditingPlace->GetGeoloc()->GetString(EGeolocLocality);
		iPostcode = iEditingPlace->GetGeoloc()->GetString(EGeolocPostalcode);
		iRegion = iEditingPlace->GetGeoloc()->GetString(EGeolocRegion);
		iCountry = iEditingPlace->GetGeoloc()->GetString(EGeolocCountry);
		iShared = iEditingPlace->Shared();
	}
}

void CBuddycloudEditPlaceList::EditCurrentItemL() {
	EditItemL(ListBox()->CurrentItemIndex(), false);
}

void CBuddycloudEditPlaceList::LoadL() {
	GetEditedPlace();	
	LoadSettingsL();
}

void CBuddycloudEditPlaceList::SaveL() {
	StoreSettingsL();

	if(iEditingPlace) {
		iEditingPlace->GetGeoloc()->SetStringL(EGeolocText, iName);
		iEditingPlace->GetGeoloc()->SetStringL(EGeolocStreet, iStreet);
		iEditingPlace->GetGeoloc()->SetStringL(EGeolocArea, iArea);
		iEditingPlace->GetGeoloc()->SetStringL(EGeolocLocality, iCity);
		iEditingPlace->GetGeoloc()->SetStringL(EGeolocPostalcode, iPostcode);
		iEditingPlace->GetGeoloc()->SetStringL(EGeolocRegion, iRegion);
		iEditingPlace->GetGeoloc()->SetStringL(EGeolocCountry, iCountry);
		iEditingPlace->SetShared(iShared);
	}
}

void CBuddycloudEditPlaceList::DynInitMenuPaneL(TInt aResourceId, CEikMenuPane* aMenuPane) {
	if(aResourceId == R_EDITPLACE_OPTIONS_MENU) {
		CAknSettingItemArray* aItemArray = SettingItemArray();
		TInt aIdentifier = ((*aItemArray)[ListBox()->CurrentItemIndex()])->Identifier();
		
		aMenuPane->SetItemDimmed(EMenuEditItemCommand, false);
		aMenuPane->SetItemDimmed(EMenuNewSearchCommand, true);
		aMenuPane->SetItemDimmed(EAknSoftkeyDone, true);
		
		if(iName.Length() > 0) {
			aMenuPane->SetItemDimmed(EMenuNewSearchCommand, false);
			aMenuPane->SetItemDimmed(EAknSoftkeyDone, false);
		}
		
		if(aIdentifier == EEditPlaceItemCountry) {
			if(iEditingPlace && iEditingPlace->GetGeoloc()->GetString(EGeolocCountry).Length() > 0) {
				aMenuPane->SetItemDimmed(EMenuEditItemCommand, true);
			}
		}
	}
}

void CBuddycloudEditPlaceList::NotificationEvent(TBuddycloudLogicNotificationType aEvent, TInt /*aId*/) {
	if(aEvent == ENotificationEditPlaceRequested) {
		LoadL();
	}
	else if(aEvent == ENotificationEditPlaceCompleted) {
		ReportEventL(MCoeControlObserver::EEventRequestExit);
	}
}

void CBuddycloudEditPlaceList::EditItemL(TInt aIndex, TBool aCalledFromMenu) {
	CAknSettingItemArray* aItemArray = SettingItemArray();
	TInt aIdentifier = ((*aItemArray)[aIndex])->Identifier();

	// Set title
	switch(aIdentifier) {
		case EEditPlaceItemName:
			SetTitleL(R_LOCALIZED_STRING_EDITPLACE_NAME);
			break;
		case EEditPlaceItemStreet:
			SetTitleL(R_LOCALIZED_STRING_EDITPLACE_STREET);
			break;
		case EEditPlaceItemArea:
			SetTitleL(R_LOCALIZED_STRING_EDITPLACE_AREA);
			break;
		case EEditPlaceItemCity:
			SetTitleL(R_LOCALIZED_STRING_EDITPLACE_CITY);
			break;
		case EEditPlaceItemPostcode:
			SetTitleL(R_LOCALIZED_STRING_EDITPLACE_POSTCODE);
			break;
		case EEditPlaceItemRegion:
			SetTitleL(R_LOCALIZED_STRING_EDITPLACE_REGION);
			break;
		case EEditPlaceItemCountry:			
			if(iEditingPlace && iEditingPlace->GetGeoloc()->GetString(EGeolocCountry).Length() > 0) {
				return;
			}
			
			SetTitleL(R_LOCALIZED_STRING_EDITPLACE_COUNTRY);
			break;
		default:;
	}
	
	CAknSettingItemList::EditItemL(aIndex, aCalledFromMenu);
	
	// Reset title
	SetTitleL(iTitleResourceId);
	
	((*aItemArray)[aIndex])->StoreL();	
	((*aItemArray)[aIndex])->UpdateListBoxTextL();	
}

CAknSettingItem* CBuddycloudEditPlaceList::CreateSettingItemL (TInt aIdentifier) {
	CAknSettingItem* aSettingItem = NULL;

    switch(aIdentifier) {
		case EEditPlaceItemName:
			aSettingItem = new (ELeave) CAknTextSettingItem(aIdentifier, iName);
			aSettingItem->SetSettingPageFlags(CAknTextSettingPage::EPredictiveTextEntryPermitted);
			break;
		case EEditPlaceItemStreet:
			aSettingItem = new (ELeave) CAknTextSettingItem(aIdentifier, iStreet);
			aSettingItem->SetSettingPageFlags(CAknTextSettingPage::EZeroLengthAllowed | CAknTextSettingPage::EPredictiveTextEntryPermitted);
			break;
		case EEditPlaceItemArea:
			aSettingItem = new (ELeave) CAknTextSettingItem(aIdentifier, iArea);
			aSettingItem->SetSettingPageFlags(CAknTextSettingPage::EZeroLengthAllowed | CAknTextSettingPage::EPredictiveTextEntryPermitted);
			break;
		case EEditPlaceItemCity:
			aSettingItem = new (ELeave) CAknTextSettingItem(aIdentifier, iCity);
			aSettingItem->SetSettingPageFlags(CAknTextSettingPage::EZeroLengthAllowed | CAknTextSettingPage::EPredictiveTextEntryPermitted);
			break;
		case EEditPlaceItemPostcode:
			aSettingItem = new (ELeave) CAknTextSettingItem(aIdentifier, iPostcode);
			aSettingItem->SetSettingPageFlags(CAknTextSettingPage::EZeroLengthAllowed | CAknTextSettingPage::EPredictiveTextEntryPermitted);
			break;
		case EEditPlaceItemRegion:
			aSettingItem = new (ELeave) CAknTextSettingItem(aIdentifier, iRegion);
			aSettingItem->SetSettingPageFlags(CAknTextSettingPage::EZeroLengthAllowed | CAknTextSettingPage::EPredictiveTextEntryPermitted);
			break;
		case EEditPlaceItemCountry:
			aSettingItem = new (ELeave) CAknTextSettingItem(aIdentifier, iCountry);
			aSettingItem->SetSettingPageFlags(CAknTextSettingPage::EPredictiveTextEntryPermitted);
			break;
		case EEditPlaceItemVisibility:
			aSettingItem = new (ELeave) CAknBinaryPopupSettingItem(aIdentifier, iShared);
			break;
		default:
			break;
    }

    return aSettingItem;
}
