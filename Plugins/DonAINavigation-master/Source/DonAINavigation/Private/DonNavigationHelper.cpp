// The MIT License(MIT)
//
// Copyright(c) 2015 Venugopalan Sreedharan
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files(the "Software"), 
// to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
// and / or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#include "DonNavigationHelper.h"
#include "DonAINavigationPrivatePCH.h"

ADonNavigationManager* UDonNavigationHelper::DonNavigationManager(UObject* WorldContextObject)
{
	UWorld* const World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull);
	if (!World)
		return NULL;

	for (TActorIterator<ADonNavigationManager> It(World, ADonNavigationManager::StaticClass()); It; ++It)
	{
		return *It;
	}

	return NULL;
}

ADonNavigationManager* UDonNavigationHelper::DonNavigationManagerForActor(const AActor *Actor)
{
	// UE_LOG(DoNNavigationLog, Log, TEXT("Other pointer val is %p with location %s"), (void *)Actor, *(Actor->GetActorLocation().ToString()));


	if (!Actor)
		return nullptr;

	TActorIterator<ADonNavigationManager> It(Actor->GetWorld(), ADonNavigationManager::StaticClass());


	// UE_LOG(DoNNavigationLog, Log, TEXT("%s"), *(Actor->GetWorld()->GetMapName()));
	// UE_LOG(DoNNavigationLog, Log, TEXT("%p %s"), &It, It ? *FString("true") : *FString("false"));

	for (; It; ++It) {
		const ADonNavigationManager *Mgr = *It;
		// UE_LOG(DoNNavigationLog, Log, TEXT("MGR pointer val is %p and it is within world is %d"), (void *)Mgr, Mgr->IsLocationWithinNavigableWorld(Actor->GetActorLocation()));
		if (Mgr->IsLocationWithinNavigableWorld(Actor->GetActorLocation()))
			return *It;
	}

	return nullptr;
}
