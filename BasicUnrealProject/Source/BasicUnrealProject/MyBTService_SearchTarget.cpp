// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBTService_SearchTarget.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetSystemLibrary.h"

UMyBTService_SearchTarget::UMyBTService_SearchTarget()
{
	NodeName = "Search Target";

    // ������ Ű�� �����մϴ�.
    TargetLocationKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UMyBTService_SearchTarget, TargetLocationKey));
}

void UMyBTService_SearchTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    // AI ��Ʈ�ѷ��� ������ ������Ʈ�� �����ɴϴ�.
    AAIController* AICon = OwnerComp.GetAIOwner();
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

    if (AICon && BlackboardComp)
    {
        // ���� ��(Pawn)�� ��ġ�� �����ɴϴ�.
        APawn* ControlledPawn = AICon->GetPawn();
        if (ControlledPawn)
        {
            UWorld* World = ControlledPawn->GetWorld();
            if (World)
            {
                FVector PawnLocation = ControlledPawn->GetActorLocation();

                // Ž�� �ݰ� ���� ��ü�� Ž���մϴ�.
                TArray<FOverlapResult> OverlapResults;
                FCollisionQueryParams QueryParams(NAME_None, false, ControlledPawn);

                // ��� �� ����� (���� ������ �׷����°� ����)
                UKismetSystemLibrary::FlushPersistentDebugLines(World);
                // ����׿����� Ž���ϴ� ������ �ð������� ǥ����.
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
                            // �����忡 Ž���� ��ü�� ��ġ�� �����մϴ�.
                            BlackboardComp->SetValueAsVector(TargetLocationKey.SelectedKeyName, OverlappedActor->GetActorLocation());

                            // ����׿����� Ž���� ��ü�� �ð������� ǥ���մϴ�.
                            DrawDebugSphere(World, OverlappedActor->GetActorLocation(), 50.0f, 12, FColor::Red, false, 1.0f);

                            //UE_LOG(LogTemp, Log, TEXT("Detected Actor: %s"), *OverlappedActor->GetName());
                            break; // ù ��° Ž���� ��ü�� ����մϴ�.
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
