//
// Created by Yasmeena Khalaf on 12/07/2026.
//

#ifndef DS_WET2_SPRING_2026_3_AVLTREE_H
#define DS_WET2_SPRING_2026_3_AVLTREE_H

#include "TreeNode.h"

template <typename K, typename V>
class AVLTree
{
private:
    TreeNode<K, V>* root;
    int nodesCount;

    static int nodeH(TreeNode<K, V>* n)
    {
        return n ? n->getHeight() : 0;
    }

    static int nodeW(TreeNode<K, V>* n)
    {
        return n ? n->getWeight() : 0;
    }

    static int bFactor(TreeNode<K, V>* n)
    {
        if (!n) return 0;
        return nodeH(n->getLeft()) - nodeH(n->getRight());
    }

    // עדכון הגובה וגודל תת-העץ של הצומת בזמן O(1)
    static void updateNode(TreeNode<K, V>* n)
    {
        if (!n) return;
        int lh = nodeH(n->getLeft());
        int rh = nodeH(n->getRight());
        n->setHeight(1 + (lh > rh ? lh : rh));
        n->setWeight(1 + nodeW(n->getLeft()) + nodeW(n->getRight()));
    }

    static TreeNode<K, V>* rotateRight(TreeNode<K, V>* y)
    {
        TreeNode<K, V>* x = y->getLeft();
        TreeNode<K, V>* z = x->getRight();

        x->setRight(y);
        y->setLeft(z);

        x->setParent(y->getParent());
        y->setParent(x);
        if (z) z->setParent(y);

        updateNode(y);
        updateNode(x);
        return x;
    }

    static TreeNode<K, V>* rotateLeft(TreeNode<K, V>* x)
    {
        TreeNode<K, V>* y = x->getRight();
        TreeNode<K, V>* z = y->getLeft();

        y->setLeft(x);
        x->setRight(z);

        y->setParent(x->getParent());
        x->setParent(y);
        if (z) z->setParent(x);

        updateNode(x);
        updateNode(y);
        return y;
    }

    static TreeNode<K, V>* rebalance(TreeNode<K, V>* node)
    {
        updateNode(node);
        int bf = bFactor(node);

        if (bf > 1)
        {
            // Left heavy
            if (bFactor(node->getLeft()) < 0)
            {
                node->setLeft(rotateLeft(node->getLeft()));
            }
            return rotateRight(node);
        }
        if (bf < -1)
        {
            // Right heavy
            if (bFactor(node->getRight()) > 0)
            {
                node->setRight(rotateRight(node->getRight()));
            }
            return rotateLeft(node);
        }
        return node;
    }

    TreeNode<K, V>* insertAux(TreeNode<K, V>* node, TreeNode<K, V>* par, const K& key, const V& value,
                              TreeNode<K, V>*& newNode)
    {
        if (!node)
        {
            node = new TreeNode<K, V>(key, value);
            node->setParent(par);
            newNode = node;
            return node;
        }
        if (key < node->getKey())
        {
            node->setLeft(insertAux(node->getLeft(), node, key, value, newNode));
        }
        else if (key > node->getKey())
        {
            node->setRight(insertAux(node->getRight(), node, key, value, newNode));
        }
        else
        {
            newNode = nullptr;
            return node;
        }
        return rebalance(node);
    }

    static TreeNode<K, V>* subtreeMin(TreeNode<K, V>* node)
    {
        if (!node) return nullptr;
        while (node->getLeft()) node = node->getLeft();
        return node;
    }

    // החלפה של שני צמתים בעץ
    void swapNodes(TreeNode<K, V>* u, TreeNode<K, V>* v)
    {
        if (!u || !v || u == v) return;

        TreeNode<K, V>* uParent = u->getParent();
        TreeNode<K, V>* uLeft = u->getLeft();
        TreeNode<K, V>* uRight = u->getRight();
        int uHeight = u->getHeight();
        int uWeight = u->getWeight();

        TreeNode<K, V>* vParent = v->getParent();
        TreeNode<K, V>* vLeft = v->getLeft();
        TreeNode<K, V>* vRight = v->getRight();
        int vHeight = v->getHeight();
        int vWeight = v->getWeight();

        if (u->getLeft() == v)
        {
            u->setLeft(vLeft);
            u->setRight(vRight);
            u->setParent(v);

            v->setLeft(u);
            v->setRight(uRight);
            v->setParent(uParent);
        }
        else if (u->getRight() == v)
        {
            u->setLeft(vLeft);
            u->setRight(vRight);
            u->setParent(v);

            v->setLeft(uLeft);
            v->setRight(u);
            v->setParent(uParent);
        }
        else
        {
            u->setLeft(vLeft);
            u->setRight(vRight);
            u->setParent(vParent);

            v->setLeft(uLeft);
            v->setRight(uRight);
            v->setParent(uParent);
        }

        if (u->getLeft())
            u->getLeft()->setParent(u);
        if (u->getRight())
            u->getRight()->setParent(u);
        if (v->getLeft())
            v->getLeft()->setParent(v);
        if (v->getRight())
            v->getRight()->setParent(v);

        if (uParent && uParent != v)
        {
            if (uParent->getLeft() == u)
                uParent->setLeft(v);
            else
                uParent->setRight(v);
        }
        if (vParent && vParent != u)
        {
            if (vParent->getLeft() == v)
                vParent->setLeft(u);
            else
                vParent->setRight(u);
        }

        if (root == u)
            root = v;
        else if (root == v)
            root = u;

        u->setHeight(vHeight);
        u->setWeight(vWeight);
        v->setHeight(uHeight);
        v->setWeight(uWeight);
    }

    TreeNode<K, V>* removeAux(TreeNode<K, V>* node, const K& key, bool& found)
    {
        if (!node)
        {
            return nullptr;
        }

        if (key < node->getKey())
        {
            TreeNode<K, V>* newLeft = removeAux(node->getLeft(), key, found);
            node->setLeft(newLeft);

            if (newLeft)
            {
                newLeft->setParent(node);
            }
        }
        else if (key > node->getKey())
        {
            TreeNode<K, V>* newRight = removeAux(node->getRight(), key, found);
            node->setRight(newRight);

            if (newRight)
            {
                newRight->setParent(node);
            }
        }
        else
        {
            found = true;
            if (node->getLeft() && node->getRight())
            {
                TreeNode<K, V>* succ = subtreeMin(node->getRight());
                swapNodes(node, succ);

                TreeNode<K, V>* newRight = removeAux(succ->getRight(), key, found);

                succ->setRight(newRight);

                if (newRight)
                {
                    newRight->setParent(succ);
                }

                return rebalance(succ);
            }

            TreeNode<K, V>* child = node->getLeft() ? node->getLeft() : node->getRight();

            if (child)
            {
                child->setParent(node->getParent());
            }

            delete node;
            return child;
        }

        return rebalance(node);
    }

    static void postFreeHelper(TreeNode<K, V>* node)
    {
        if (!node) return;
        postFreeHelper(node->getLeft());
        postFreeHelper(node->getRight());
        delete node;
    }

    TreeNode<K, V>* findIthAux(TreeNode<K, V>* node, int index) const
    {
        if (!node) return nullptr;

        int leftWeight = nodeW(node->getLeft());
        if (index == leftWeight + 1)
        {
            return node;
        }
        if (index <= leftWeight)
        {
            return findIthAux(node->getLeft(), index);
        }

        return findIthAux(node->getRight(), index - leftWeight - 1);
    }

public:
    AVLTree() : root(nullptr), nodesCount(0)
    {
    }

    ~AVLTree()
    {
        postOrderFree();
    }

    AVLTree(const AVLTree&) = delete;
    AVLTree& operator=(const AVLTree&) = delete;

    TreeNode<K, V>* insert(const K& key, const V& value)
    {
        TreeNode<K, V>* newNode = nullptr;
        root = insertAux(root, nullptr, key, value, newNode);
        if (root) root->setParent(nullptr);
        if (newNode) nodesCount++;
        return newNode;
    }

    bool remove(const K& key)
    {
        bool found = false;
        root = removeAux(root, key, found);
        if (root) root->setParent(nullptr);
        if (found)
            nodesCount--;
        return found;
    }

    TreeNode<K, V>* find(const K& key) const
    {
        TreeNode<K, V>* cur = root;
        while (cur)
        {
            if (key < cur->getKey()) cur = cur->getLeft();
            else if (key > cur->getKey()) cur = cur->getRight();
            else return cur;
        }
        return nullptr;
    }

    // שליפת האיבר ה-i בעץ בזמן O(log n) לפי הדרגות (Rank Tree) עבור get_ith_collective_motivation_team
    TreeNode<K, V>* findIth(int i) const
    {
        if (i < 1 || i > nodesCount) return nullptr;
        return findIthAux(root, i);
    }

    void postOrderFree()
    {
        postFreeHelper(root);
        root = nullptr;
        nodesCount = 0;
    }

    TreeNode<K, V>* getRoot() const { return root; }
    int getSize() const { return nodesCount; }
    bool isEmpty() const { return root == nullptr; }
};

#endif //DS_WET2_SPRING_2026_3_AVLTREE_H
