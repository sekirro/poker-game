#include "UndoModel.h"

UndoModel::UndoModel()
{
}

UndoModel::~UndoModel()
{
    clear();
}

void UndoModel::pushAction(const UndoAction& action)
{
    _actions.push_back(action);
}

UndoAction UndoModel::popAction()
{
    if (_actions.empty()) {
        return UndoAction();
    }
    
    UndoAction action = _actions.back();
    _actions.pop_back();
    return action;
}

void UndoModel::clear()
{
    _actions.clear();
}

bool UndoModel::canUndo() const
{
    return !_actions.empty();
}

int UndoModel::getActionCount() const
{
    return static_cast<int>(_actions.size());
} 