// Fill out your copyright notice in the Description page of Project Settings.

#include "MyBTService_SearchTarget.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetSystemLibrary.h"

UMyBTService_SearchTarget::UMyBTService_SearchTarget()
{
    NodeName = "Search Target";    // ����� �̸��� "Search Target"���� �����մϴ�.
    Interval = 1.0f;               // ���񽺰� ����Ǵ� ������ 1.0�ʷ� �����մϴ�.

    // ������ Ű�� �����մϴ�. (TargetLocationKey)
    TargetLocationKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UMyBTService_SearchTarget, TargetLocationKey)); 
    
    // ������ Ű�� �����մϴ�. (TargetDistance)
    TargetDistanceKey.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(UMyBTService_SearchTarget, TargetDistanceKey));
}

// TickNode �Լ�: ���񽺰� ����� ������ ȣ��˴ϴ�.
void UMyBTService_SearchTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
 
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    //UE_LOG(LogTemp, Log, TEXT("BT_Service DeltaSeconds: %f"), DeltaSeconds); // ����� �α׿� DeltaSeconds�� ����մϴ�.

    AAIController* AICon = OwnerComp.GetAIOwner();  // AI ��Ʈ�ѷ��� ������ ������Ʈ�� �����ɴϴ�.
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

    // AI ��Ʈ�ѷ��� ������ ������Ʈ�� ��ȿ���� Ȯ���մϴ�.
    if (AICon && BlackboardComp)
    {
        APawn* ControlledPawn = AICon->GetPawn(); // ���� ��(Pawn)�� ��ġ�� �����ɴϴ�.
        if (ControlledPawn)
        {
            
            UWorld* World = ControlledPawn->GetWorld(); // ����(World) ��ü�� �����ɴϴ�.
            if (World)
            {
                // ���� ���� ��ġ�� �����ɴϴ�.
                FVector PawnLocation = ControlledPawn->GetActorLocation();

                TArray<FOverlapResult> OverlapResults; // Ž�� �ݰ� ���� ��ü�� Ž���մϴ�.

                UKismetSystemLibrary::FlushPersistentDebugLines(World); // ��� �� ����� (���� ���� �� �׷����� ���� ����)
                
                // Ž���� ������Ʈ Ÿ���� �����մϴ�. (���⼭�� Pawn ä���� ���)
                FCollisionObjectQueryParams ObjectQueryParams; 
                ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);

                // OverlapMultiByObjectType �Լ��� ����Ͽ� Ž�� �ݰ� ���� ��ü�� Ž���մϴ�.
                bool bIsOverlapping = World->OverlapMultiByObjectType(
                    OverlapResults,
                    PawnLocation,
                    FQuat::Identity,
                    ObjectQueryParams,
                    FCollisionShape::MakeSphere(SearchRadius)
                );

                // ����׿����� Ž���ϴ� ������ �ð������� ǥ���մϴ�.
                DrawDebugSphere(World, PawnLocation, SearchRadius, 12, FColor::Red, true, -1.0f);

                // ������ Ű�� ���� �ʱ�ȭ�մϴ�. �ʱ�ȭ �� ���·� ������ ��� ���ڷ����Ϳ��� Is Not Set ���� �Ǵ� �˴ϴ� 
                BlackboardComp->ClearValue(TargetLocationKey.SelectedKeyName);
                BlackboardComp->ClearValue(TargetDistanceKey.SelectedKeyName);

                // Ž���� ��ü�� �ִ��� Ȯ���մϴ�.
                if (bIsOverlapping)
                {
                    for (auto& Result : OverlapResults)
                    {
                        AActor* OverlappedActor = Result.GetActor();
                        // Ž���� ��ü�� �ڽ��� �ƴ��� Ȯ���մϴ�.
                        if (OverlappedActor && OverlappedActor != ControlledPawn)
                        {
                            // ����׿����� Ž���ϴ� ������ �ð������� ǥ���մϴ�. (������ �� ��� ��������� �ٲߴϴ�)
                            DrawDebugSphere(World, PawnLocation, SearchRadius, 12, FColor::Yellow, true, -1.0f);

                            // �����忡 Ž���� ��ü�� ��ġ�� �����մϴ�.
                            BlackboardComp->SetValueAsVector(TargetLocationKey.SelectedKeyName, OverlappedActor->GetActorLocation());

                            // ����׿����� Ž���� ��ü�� �ð������� ǥ���մϴ�.
                            DrawDebugSphere(World, OverlappedActor->GetActorLocation(), 50.0f, 12, FColor::Red, false, 1.0f);

                            // �����忡 Ž���� ��ü���� �Ÿ��� �����մϴ�.
                            BlackboardComp->SetValueAsFloat(TargetDistanceKey.SelectedKeyName, FVector::Dist(OverlappedActor->GetActorLocation(), PawnLocation));

                            UE_LOG(LogTemp, Log, TEXT("Distance : %f"), FVector::Dist(OverlappedActor->GetActorLocation(), PawnLocation));
                            // ����� �α׿� Ž���� ��ü�� �̸��� ����մϴ�.
                            //UE_LOG(LogTemp, Log, TEXT("Detected Actor: %s"), *OverlappedActor->GetName());
                            break; // ù ��° Ž���� ��ü�� ����մϴ�.
                        }
                    }
                }

                //����׿�, ���� ������ Ÿ���� ��ǥ�� ����մϴ�. 
                //FVector TargetLocation = BlackboardComp->GetValueAsVector(TargetLocationKey.SelectedKeyName);
                //UE_LOG(LogTemp, Log, TEXT("TargetLocationKey: %s"), *TargetLocation.ToString());
            }
            else
            {
                // ���� ��ü�� nullptr�� ��� ��� �α׸� ����մϴ�.
                UE_LOG(LogTemp, Warning, TEXT("World is nullptr in TickNode."));
            }
        }
        else
        {
            // ��Ʈ�ѵ� ���� nullptr�� ��� ��� �α׸� ����մϴ�.
            UE_LOG(LogTemp, Warning, TEXT("ControlledPawn is nullptr in TickNode."));
        }
    }
    else
    {
        // AI ��Ʈ�ѷ� �Ǵ� ������ ������Ʈ�� nullptr�� ��� ��� �α׸� ����մϴ�.
        UE_LOG(LogTemp, Warning, TEXT("AIController or BlackboardComp is nullptr in TickNode."));
    }
}