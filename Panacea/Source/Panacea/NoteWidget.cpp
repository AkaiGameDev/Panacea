#include "NoteWidget.h"

void UNoteWidget::SetNoteActor(ANoteActor* InNoteActor)
{
	NoteActor = InNoteActor;
}

ANoteActor* UNoteWidget::GetNoteActor() const
{
	return NoteActor;
}

