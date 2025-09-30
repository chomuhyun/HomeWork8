#include "DroneController.h"
#include "MyGameState.h"
#include "MyGameInstance.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"

ADroneController::ADroneController()
	:InputMappingContext(nullptr),
	IA_MoveFwd(nullptr),
	IA_MoveUp(nullptr),
	IA_MoveDown(nullptr),
	IA_Look(nullptr),
	IA_Roll(nullptr),
	IA_Sprint(nullptr),
	HUDWidgetClass(nullptr),
	HUDWidgetInstance(nullptr),
	MainMenuHUDWidgetClass(nullptr),
	MainMenuHUDWidgetInstance(nullptr)
{
}

void ADroneController::BeginPlay()
{
	Super::BeginPlay();

	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* SubSystem =
			LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (InputMappingContext)
			{
				SubSystem->AddMappingContext(InputMappingContext, 0);
			}
		}
	}
	FString CurrentMapName = GetWorld()->GetMapName();
	if (CurrentMapName.Contains("MenuLevel"))
	{
		ShowMainMenu(false);
	}
}

UUserWidget* ADroneController::GetHUDWidget() const
{
	return HUDWidgetInstance;
}

void ADroneController::ShowMainMenu(bool bIsRestart)
{
	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->RemoveFromParent();
		HUDWidgetInstance = nullptr;
	}

	if (MainMenuHUDWidgetInstance)
	{
		MainMenuHUDWidgetInstance->RemoveFromParent();
		MainMenuHUDWidgetInstance = nullptr;
	}

	if (MainMenuHUDWidgetClass)
	{
		MainMenuHUDWidgetInstance = CreateWidget<UUserWidget>(this, MainMenuHUDWidgetClass);
		if (MainMenuHUDWidgetInstance)
		{
			MainMenuHUDWidgetInstance->AddToViewport();

			bShowMouseCursor = true;
			SetInputMode(FInputModeUIOnly());
		}

		if (UTextBlock* ButtonText = Cast<UTextBlock>(MainMenuHUDWidgetInstance->GetWidgetFromName(TEXT("StartButtonText"))))
		{
			if (bIsRestart)
			{
				ButtonText->SetText(FText::FromString(TEXT("Restart")));
			}
			else
			{
				UFunction* PlayStart = MainMenuHUDWidgetInstance->FindFunction(FName("StartPlay"));
				if (PlayStart)
				{
					MainMenuHUDWidgetInstance->ProcessEvent(PlayStart, nullptr);
				}
				ButtonText->SetText(FText::FromString(TEXT("Start")));
			}
		}
		if (UTextBlock* ButtonText = Cast<UTextBlock>(MainMenuHUDWidgetInstance->GetWidgetFromName(TEXT("ExitButtonText"))))
		{
			if (UButton* ExitButton = Cast<UButton>(MainMenuHUDWidgetInstance->GetWidgetFromName(TEXT("ExitButton"))))
			{
				ExitButton->OnClicked.Clear();
				if (bIsRestart)
				{
					ExitButton->OnClicked.Clear();
					ButtonText->SetText(FText::FromString(TEXT("Menu")));
					ExitButton->OnClicked.AddDynamic(this, &ADroneController::ReturnToMenu);
				}
				else
				{
					ExitButton->OnClicked.Clear();
					ButtonText->SetText(FText::FromString(TEXT("Exit")));
					ExitButton->OnClicked.AddDynamic(this, &ADroneController::ExitGame);
				}
			}
		}

		if (bIsRestart)
		{
			UFunction* PlayeAnimFunc = MainMenuHUDWidgetInstance->FindFunction(FName("PlayGameOverAnim"));
			if (PlayeAnimFunc)
			{
				MainMenuHUDWidgetInstance->ProcessEvent(PlayeAnimFunc, nullptr);
			}

			if (UTextBlock* TotalScoreText = Cast<UTextBlock>(MainMenuHUDWidgetInstance->GetWidgetFromName("TotalScoreText")))
			{
				if (UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(this)))
				{
					TotalScoreText->SetText(FText::FromString(
						FString::Printf(TEXT("Total Score : %d"), MyGameInstance->TotalScore)
					));
				}
			}
		}
	}
}

void ADroneController::ShowGameHUD()
{
	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->RemoveFromParent();
		HUDWidgetInstance = nullptr;
	}

	if (MainMenuHUDWidgetInstance)
	{
		MainMenuHUDWidgetInstance->RemoveFromParent();
		MainMenuHUDWidgetInstance = nullptr;
	}

	if (HUDWidgetClass)
	{
		HUDWidgetInstance = CreateWidget<UUserWidget>(this, HUDWidgetClass);
		if (HUDWidgetInstance)
		{
			HUDWidgetInstance->AddToViewport();

			bShowMouseCursor = false;
			SetInputMode(FInputModeGameOnly());
		}

		AMyGameState* MyGameState = GetWorld() ? GetWorld()->GetGameState<AMyGameState>() : nullptr;
		if (MyGameState)
		{
			MyGameState->UpdateHUD();
		}
	}
}

void ADroneController::StartGame()
{
	if (UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		MyGameInstance->CurrentLevelIndex = 0;
		MyGameInstance->TotalScore = 0;
	}

	UGameplayStatics::OpenLevel(GetWorld(), FName("JumpGame"));
	SetPause(false);
}

void ADroneController::ExitGame()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, true);
}

void ADroneController::ReturnToMenu()
{
	SetPause(false);
	UGameplayStatics::OpenLevel(GetWorld(), FName("MenuLevel"));
}
