// Fill out your copyright notice in the Description page of Project Settings.

#include "MyBTService_SearchTarget.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetSystemLibrary.h"

UMyBTService_SearchTarget::UMyBTService_SearchTarget()
{
    NodeName = "Search Target";    // 노드의 이름을 "Search Target"으로 설정합니다.
    Interval = 1.0f;               // 서비스가 실행되는 간격을 1.0초로 설정합니다.

    // 블랙보드 키를 설정합니다.
    TargetLocationKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UMyBTService_SearchTarget, TargetLocationKey));
}

// TickNode 함수: 서비스가 실행될 때마다 호출됩니다.
void UMyBTService_SearchTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
 
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    UE_LOG(LogTemp, Log, TEXT("DeltaSeconds: %f"), DeltaSeconds); // 디버그 로그에 DeltaSeconds를 출력합니다.

    AAIController* AICon = OwnerComp.GetAIOwner();  // AI 컨트롤러와 블랙보드 컴포넌트를 가져옵니다.
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

    // AI 컨트롤러와 블랙보드 컴포넌트가 유효한지 확인합니다.
    if (AICon && BlackboardComp)
    {
        APawn* ControlledPawn = AICon->GetPawn(); // 현재 폰(Pawn)의 위치를 가져옵니다.
        if (ControlledPawn)
        {
            
            UWorld* World = ControlledPawn->GetWorld(); // 월드(World) 객체를 가져옵니다.
            if (World)
            {
                // 폰의 현재 위치를 가져옵니다.
                FVector PawnLocation = ControlledPawn->GetActorLocation();

                TArray<FOverlapResult> OverlapResults; // 탐색 반경 내의 객체를 탐지합니다.

                UKismetSystemLibrary::FlushPersistentDebugLines(World); // 모든 선 지우기 (선이 여러 개 그려지는 것을 방지)
                
                // 탐색할 오브젝트 타입을 설정합니다. (여기서는 Pawn 채널을 사용)
                FCollisionObjectQueryParams ObjectQueryParams; 
                ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);

                // OverlapMultiByObjectType 함수를 사용하여 탐색 반경 내의 객체를 탐지합니다.
                bool bIsOverlapping = World->OverlapMultiByObjectType(
                    OverlapResults,
                    PawnLocation,
                    FQuat::Identity,
                    ObjectQueryParams,
                    FCollisionShape::MakeSphere(SearchRadius)
                );

                // 디버그용으로 탐지하는 범위를 시각적으로 표시합니다.
                DrawDebugSphere(World, PawnLocation, SearchRadius, 12, FColor::Red, true, -1.0f);

                // 탐지된 객체가 있는지 확인합니다.
                if (bIsOverlapping)
                {
                    for (auto& Result : OverlapResults)
                    {
                        AActor* OverlappedActor = Result.GetActor();
                        // 탐지된 객체가 자신이 아닌지 확인합니다.
                        if (OverlappedActor && OverlappedActor != ControlledPawn)
                        {
                            // 디버그용으로 탐지하는 범위를 시각적으로 표시합니다. (감지가 된 경우 노란색으로 바꿉니다)
                            DrawDebugSphere(World, PawnLocation, SearchRadius, 12, FColor::Yellow, true, -1.0f);

                            // 블랙보드에 탐지된 객체의 위치를 저장합니다.
                            BlackboardComp->SetValueAsVector(TargetLocationKey.SelectedKeyName, OverlappedActor->GetActorLocation());

                            // 디버그용으로 탐지된 객체를 시각적으로 표시합니다.
                            DrawDebugSphere(World, OverlappedActor->GetActorLocation(), 50.0f, 12, FColor::Red, false, 1.0f);

                            // 디버그 로그에 탐지된 객체의 이름을 출력합니다.
                            UE_LOG(LogTemp, Log, TEXT("Detected Actor: %s"), *OverlappedActor->GetName());
                            break; // 첫 번째 탐지된 객체를 사용합니다.
                        }
                    }
                }
            }
            else
            {
                // 월드 객체가 nullptr인 경우 경고 로그를 출력합니다.
                UE_LOG(LogTemp, Warning, TEXT("World is nullptr in TickNode."));
            }
        }
        else
        {
            // 컨트롤된 폰이 nullptr인 경우 경고 로그를 출력합니다.
            UE_LOG(LogTemp, Warning, TEXT("ControlledPawn is nullptr in TickNode."));
        }
    }
    else
    {
        // AI 컨트롤러 또는 블랙보드 컴포넌트가 nullptr인 경우 경고 로그를 출력합니다.
        UE_LOG(LogTemp, Warning, TEXT("AIController or BlackboardComp is nullptr in TickNode."));
    }
}