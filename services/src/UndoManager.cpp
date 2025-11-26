#include "UndoManager.h"
#include <iostream>

void UndoManager::addAction(function<void()> undoAction)
{
    if (undoStack.size() >= MAX_UNDO_STEPS)
    {
        undoStack.erase(undoStack.begin());
    }
    undoStack.push_back(undoAction);
}

bool UndoManager::undo()
{
    if (undoStack.empty())
    {
        return false;
    }
    
    auto action = undoStack.back();
    undoStack.pop_back();
    action();
    return true;
}

void UndoManager::clear()
{
    undoStack.clear();
}

bool UndoManager::canUndo() const
{
    return !undoStack.empty();
}