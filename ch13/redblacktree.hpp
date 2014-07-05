/***************************************************************************
 *  @file       redblacktree.hpp
 *  @author     Alan.W
 *  @date       05  July 2014
 *  @remark     Algorithims implementation for CLRS, using C++ templates.
 ***************************************************************************/
#ifndef REDBLACKTREE_HPP
#define REDBLACKTREE_HPP
#include <memory>
#include <functional>
#include "node.hpp"


namespace ch13{

/**
 * @brief The RedBlackTree class
 */
template<typename K, typename D>
class RedBlackTree
{
public:
    using KeyType   =   K;
    using DataType  =   D;
    using NodeType  =   Node<K,D>;
    using sPointer  =   std::shared_ptr<NodeType>;
    using wPointer  =   std::weak_ptr<NodeType>;

    RedBlackTree():
        nil(std::make_shared<NodeType>())
    {
        root = nil;
    }

    /**
     * @brief print
     *
     * O(n)
     */
    void print()const
    {
        std::cout << "-----------------------------\n";
        std::cout << debug::yellow("root= ") << root
                  << " key = " << root->key    << std::endl;

        std::cout << debug::yellow("nil = ") << nil
                  << std::endl;

        std::function<void(sPointer node)> inorder_walk =
                [&inorder_walk,this](sPointer node)
        {
            if(node != nil)
            {
                inorder_walk(node->left);
                node->print();
                inorder_walk(node->right);
            }
        };
        inorder_walk(root);
    }

    /**
     * @brief insert
     * @param key
     *
     * O(h)
     */
    void insert(const KeyType& key)
    {
        sPointer added = std::make_shared<NodeType>(key);
        insert(added);
    }

    /**
     * @brief insert
     * @param added
     *
     * O(h)
     */
    void insert(sPointer added)
    {
        sPointer tracker = nil;
        sPointer curr = root;
        while(curr != nil)
        {
            tracker =   curr;
            curr    =   curr->key > added->key?     curr->left  :   curr->right;
        }

        added->parent   =   tracker;
        if(tracker == nil)
            root    =   added;
        else
            (added->key < tracker->key?     tracker->left   :   tracker->right)
                    =   added;

        added->left =   added->right    =   nil;
        added->color=   Color::RED;

        //--to do   :   add fixup
    }

    sPointer search(const KeyType& key)const
    {
        sPointer ret  = nullptr;
        sPointer curr = this->root;
        while(curr)
        {
            if(curr->key    ==  key)
            {
                ret = curr;
                break;
            }
            else
                curr = (curr->key > key?    curr->left  :   curr->right);
        }
        return ret;
    }

    /**
     * @brief left_rotate
     * @param x
     *
     *      [x]                    [y]
     *         \        =>        /
     *          [y]            [x]
     *
     * @complx  o(1)
     */
    void left_rotate(sPointer x)
    {
        sPointer y = x->right;      //  set y

        x->right    =   y->left;    //  turn y's left into x's right
        if(y->left  !=  this->nil)
            y->left->parent = x;

        y->parent   =   x->parent;  //  link x's parent to y
        if(x->parent.lock()    ==  this->nil)
            this->root  =   y;
        else
            (x->is_left()?  x->parent.lock()->left  :  x->parent.lock()->right)
                        =   y;

        y->left     =   x;
        x->parent   =   y;
    }

    /**
     * @brief right_rotate
     * @param y
     *
     *          [y]             [x]
     *          /       =>         \
     *        [x]                   [y]
     *
     * @complx  O(1)
     */
    void right_rotate(sPointer y)
    {
        sPointer x = y->left;

        y->left = x->right;
        if(x->right != this->nil)
            x->right->parent    =   y;

        x->parent   =   y->parent;
        if(y->parent.lock() ==  this->nil)
            root    =   x;
        else
            (y->is_left()?  y->parent.lock()->left  :  y->parent.lock()->right)
                    =   x;

        x->right    =   y;
        y->parent   =   x;
    }
private:
    sPointer root;
    sPointer nil;


    void insert_fixup(sPointer added)
    {
        while(added->ascend(1)->color   ==  Color::RED)
        {
            if(added->ascend(1)->is_left())
            {
                sPointer uncle = added->ascend(2)->right;
                if(uncle->color ==  Color::RED)
                {
                    uncle->color            =   Color::BLACK;
                    added->ascend(1)->color =   Color::BLACK;
                    added->ascend(2)->color =   Color::RED;
                    added   =   added->ascend(2);
                }
                else
                {
                    if(added->is_right())
                    {
                        added   =   added->ascend(1);
                        left_rotate(added);
                    }
                    added->ascend(1)->color =   Color::BLACK;
                    added->ascend(2)->color =   Color::RED;
                    right_rotate(added->ascend(2));
                }
            }
            else
            {
                sPointer uncle = added->ascend(2)->left;
                if(uncle->color ==  Color::RED)
                {
                    uncle->color            =   Color::BLACK;
                    added->ascend(1)->color =   Color::BLACK;
                    added->ascend(2)->color =   Color::RED;
                    added   =   added->ascend(2);
                }
                else
                {
                    if(added->is_left())
                    {
                        added   =   added->ascend(1);
                        right_rotate(added);
                    }
                    added->ascend(1)->color =   Color::BLACK;
                    added->ascend(2)->color =   Color::RED;
                    left_rotate(added->ascend(2));
                }
            }
        }
        root->color = Color::BLACK;
    }
};
}//namespace


#endif // REDBLACKTREE_HPP