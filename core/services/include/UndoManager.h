#ifndef UNDOMANAGER_H
#define UNDOMANAGER_H

#include <vector>
#include <functional>
#include <string>

using namespace std;

class UndoManager
{
private:
    vector<function<void()>> undoStack;
    static const size_t MAX_UNDO_STEPS = 10;

public:
    void addAction(function<void()> undoAction);
    bool undo();
    void clear();
    bool canUndo() const;
};

#endif