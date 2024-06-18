// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBTService_SearchTarget.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetSystemLibrary.h"

UMyBTService_SearchTarget::UMyBTService_SearchTarget()
{
	NodeName = "Search Target";

    // 블랙보드 키를 설정합니다.
    TargetLocationKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UMyBTService_SearchTarget, TargetLocationKey));
}

void UMyBTService_SearchTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    // AI 컨트롤러와 블랙보드 컴포넌트를 가져옵니다.
    AAIController* AICon = OwnerComp.GetAIOwner();
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

    if (AICon && BlackboardComp)
    {
        // 현재 폰(Pawn)의 위치를 가져옵니다.
        APawn* ControlledPawn = AICon->GetPawn();
        if (ControlledPawn)
        {
            UWorld* World = ControlledPawn->GetWorld();
            if (World)
            {
                FVector PawnLocation = ControlledPawn->GetActorLocation();

                // 탐색 반경 내의 객체를 탐지합니다.
                TArray<FOverlapResult> OverlapResults;
                FCollisionQueryParams QueryParams(NAME_None, false, ControlledPawn);

                // 모든 선 지우기 (선이 여러개 그려지는걸 방지)
                UKismetSystemLibrary::FlushPersistentDebugLines(World);
                // 디버그용으로 탐지하는 범위를 시각적으로 표시함.
                DrawDebugSphere(World, PawnLocation, SearchRadius, 12, FColor::Yellow, true, -1.0f);

                bool bIsOverlapping = World->OverlapMultiByChannel(
                    OverlapResults,
                    PawnLocation,
                    FQuat::Identity,
                    ECollisionChannel::ECC_Pawn,
                    FCollisionShape::MakeSphere(SearchRadius),
                    QueryParams
                );

                if (bIsOverlapping)
                {
                    for (auto& Result : OverlapResults)
                    {
                        AActor* OverlappedActor = Result.GetActor();
                        if (OverlappedActor && OverlappedActor != ControlledPawn)
                        {
                            // 블랙보드에 탐지된 객체의 위치를 저장합니다.
                            BlackboardComp->SetValueAsVector(TargetLocationKey.SelectedKeyName, OverlappedActor->GetActorLocation());

                            // 디버그용으로 탐지된 객체를 시각적으로 표시합니다.
                            DrawDebugSphere(World, OverlappedActor->GetActorLocation(), 50.0f, 12, FColor::Red, false, 1.0f);

                            //UE_LOG(LogTemp, Log, TEXT("Detected Actor: %s"), *OverlappedActor->GetName());
                            break; // 첫 번째 탐지된 객체를 사용합니다.
                        }
                    }
                }
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("World is nullptr in TickNode."));
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("ControlledPawn is nullptr in TickNode."));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("AIController or BlackboardComp is nullptr in TickNode."));
    }
}
