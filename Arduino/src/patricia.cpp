#include "patricia.h"

TPatricia::TPatricia() {}

short TPatricia::Insert(const string &word, TCallbackHandlerFunction val) {
  if (Empty()) {
    Left = this;
    Number = 0;
    Elem = new TKvelem(word, val);
    return 0;
  }
  TPatricia *endNode = Search(word);
  const string &reachedKey = endNode->Elem->GetKey();
  if (word == reachedKey) {
    return -1;
  }
  int lBitPos = -1;
  for (unsigned short i = 0; i < reachedKey.length() && i < word.length();
       i++) {
    if (reachedKey[i] != word[i]) {
      for (size_t j = 0; j < 8; j++) {
        if (((reachedKey[i] >> j) & 1u) != ((word[i] >> j) & 1u)) {
          lBitPos = (i << 3) + j + 1;
          break;
        }
      }
      break;
    }
  }
  if (lBitPos == -1) {
    unsigned short firstUnEqualBit = (word.length() > reachedKey.length())
                                         ? reachedKey.length()
                                         : word.length();
    if (firstUnEqualBit == word.length()) {
      for (size_t j = 0; j < 8; j++) {
        if ((reachedKey[firstUnEqualBit] >> j) & 1u) {
          lBitPos = (firstUnEqualBit << 3) + j + 1;
          break;
        }
      }
    } else {
      for (size_t j = 0; j < 8; j++) {
        if ((word[firstUnEqualBit] >> j) & 1u) {
          lBitPos = (firstUnEqualBit << 3) + j + 1;
          break;
        }
      }
    }
  }

  TPatricia *newNode = new TPatricia();
  newNode->Number = lBitPos;
  newNode->Elem = new TKvelem(word, val);
  if (GetBit(word, lBitPos - 1)) {
    newNode->Right = newNode;
    newNode->Left = NULL;
  } else {
    newNode->Left = newNode;
    newNode->Right = NULL;
  }
  if (Left == this) {
    (newNode->Left == NULL) ? newNode->Left = this : newNode->Right = this;
    Left = newNode;
    return 0;
  }
  TPatricia *p, *q;
  p = Left;
  unsigned short currentBit = p->Number, lastBit = currentBit;
  do {
    if (currentBit > lBitPos) {
      p = this;
      break;
    }
    if (word.length() << 3 < currentBit) {
      if (p->Left->Number > lBitPos || p->Left->Number < currentBit) {
        break;
      }
      p = p->Left;
    } else {
      if (GetBit(word, currentBit - 1)) {
        if (p->Right->Number > lBitPos || p->Right->Number < currentBit) {
          break;
        }
        p = p->Right;
      } else {
        if (p->Left->Number > lBitPos || p->Left->Number < currentBit) {
          break;
        }
        p = p->Left;
      }
    }
    lastBit = currentBit;
    currentBit = p->Number;
  } while (currentBit > lastBit);

  if (p == this) {
    (newNode->Left == NULL) ? newNode->Left = this->Left : newNode->Right =
                                                               this->Left;
    p->Left = newNode;
    return 0;
  }
  if (GetBit(word, currentBit - 1)) {
    q = p->Right;
    (newNode->Left == NULL) ? newNode->Left = q : newNode->Right = q;
    p->Right = newNode;
  } else {
    q = p->Left;
    (newNode->Left == NULL) ? newNode->Left = q : newNode->Right = q;
    p->Left = newNode;
  }
  return 0;
}
short TPatricia::Remove(const string &word) {
  if (Empty()) {
    return -1;
  }
  TPatricia *nodeToRemove = Search(word);
  if (nodeToRemove->Elem->GetKey() != word) {
    return -1;
  }
  if (nodeToRemove->Left == nodeToRemove ||
      nodeToRemove->Right == nodeToRemove) {
    if (nodeToRemove == this) {
      nodeToRemove->Left = NULL;
      nodeToRemove->Right = NULL;
      delete nodeToRemove->Elem;
      return 0;
    } else {
      TPatricia *nonSelfNode, *parentNodeToRemove;
      parentNodeToRemove = SearchParent(nodeToRemove);
      if (nodeToRemove->Left == nodeToRemove) {
        nonSelfNode = nodeToRemove->Right;
      } else {
        nonSelfNode = nodeToRemove->Left;
      }

      if (parentNodeToRemove->Left == nodeToRemove) {
        parentNodeToRemove->Left = nonSelfNode;
      } else {
        parentNodeToRemove->Right = nonSelfNode;
      }
      delete nodeToRemove->Elem;
    }
    delete nodeToRemove;
  } else {
    TPatricia *backwardNodeToRemove, *backwardRNode,
        *backwardNodeToRemoveParent, *tmplPointer;
    backwardNodeToRemove = SearchBackwardPointer(nodeToRemove);
    delete nodeToRemove->Elem;
    nodeToRemove->Elem = backwardNodeToRemove->Elem;
    backwardRNode = SearchBackwardPointer(backwardNodeToRemove);
    if (backwardRNode->Left == backwardNodeToRemove) {
      backwardRNode->Left = nodeToRemove;
    } else {
      backwardRNode->Right = nodeToRemove;
    }
    if (backwardNodeToRemove->Left->Number >= backwardNodeToRemove->Number) {
      tmplPointer = backwardNodeToRemove->Left;
    } else if (backwardNodeToRemove->Right->Number >=
               backwardNodeToRemove->Number) {
      tmplPointer = backwardNodeToRemove->Right;
    } else {
      tmplPointer = nodeToRemove;
    }
    backwardNodeToRemoveParent = SearchParent(backwardNodeToRemove);
    if (backwardNodeToRemoveParent->Left == backwardNodeToRemove) {
      backwardNodeToRemoveParent->Left = tmplPointer;
    } else {
      backwardNodeToRemoveParent->Right = tmplPointer;
    }
    delete backwardNodeToRemove;
  }
  return 0;
}
TPatricia *TPatricia::Search(const string &word) {
  if (Empty()) {
    return NULL;
  }
  if (Left == this) {
    return this;
  }
  TPatricia *currentNode = Left;
  unsigned short currentBit = currentNode->Number, lastBit = currentBit;
  do {
    if (word.length() << 3 < currentBit) {
      currentNode = currentNode->Left;
    } else {
      if (GetBit(word, currentBit - 1)) {
        currentNode = currentNode->Right;
      } else {
        currentNode = currentNode->Left;
      }
    }
    lastBit = currentBit;
    currentBit = currentNode->Number;
  } while (currentBit > lastBit);
  return currentNode;
}
TPatricia *TPatricia::SearchParent(TPatricia *root) {
  if (Empty()) {
    return NULL;
  }
  if (Left == root) {
    return this;
  }
  TPatricia *currentNode = Left;
  const std::string &word = root->Elem->GetKey();
  unsigned short currentBit = currentNode->Number;
  while (currentNode->Left != root && currentNode->Right != root) {
    if ((word.length() << 3) < currentBit) {
      currentNode = currentNode->Left;
    } else {
      if (GetBit(word, currentBit - 1)) {
        currentNode = currentNode->Right;
      } else {
        currentNode = currentNode->Left;
      }
    }
    currentBit = currentNode->Number;
  }
  return currentNode;
}
TPatricia *TPatricia::SearchBackwardPointer(TPatricia *root) {
  if (Empty()) {
    return NULL;
  }
  if (root->Left == root || root->Right == root) {
    return root;
  }
  TPatricia *currentNode = root;
  const std::string &word = root->Elem->GetKey();
  unsigned short currentBit = currentNode->Number;
  do {
    if ((word.length() << 3) < currentBit) {
      currentNode = currentNode->Left;
    } else {
      if (GetBit(word, currentBit - 1)) {
        currentNode = currentNode->Right;
      } else {
        currentNode = currentNode->Left;
      }
    }
    currentBit = currentNode->Number;
  } while (currentNode->Left != root && currentNode->Right != root);
  return currentNode;
}
bool TPatricia::Empty() { return Left == NULL && Right == NULL; }

unsigned short TPatricia::GetBit(const string &word, unsigned short bit) {
  int correctChar = bit >> 3;
  int correctCharBit = bit & 7;
  if ((word.length() << 3) < bit) {
    return 0;
  }
  return (word[correctChar] >> correctCharBit) & 1u;
}


short TPatricia::Destroy() {
  if (Empty()) {
    return 0;
  }
  TPatricia *nodeToDel = Left;
  while (Left != this) {
    Remove(nodeToDel->Elem->GetKey());
    nodeToDel = Left;
  }
  Remove(this->Elem->GetKey());
  return 0;
}
TPatricia::~TPatricia() {}
