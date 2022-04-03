template <typename T>
class AVLnode
{
public:
    T key;
    int balance = 0, height = 1;
    AVLnode* left = nullptr;
    AVLnode* right = nullptr;
    AVLnode* parent = nullptr;

    AVLnode(T k, AVLnode* p = nullptr) : key(k), parent(p) { }
    AVLnode(T k, int h, int b) : key(k), height(h), balance(b) { }

    ~AVLnode()
    {
        delete left;
        delete right;
    }

    void makeLeaf()
    {
        left = nullptr;
        right = nullptr;
    }

    AVLnode* nextNode()
    {
        AVLnode* ans = this;
        if (right != nullptr)
        {
            ans = right;
            while (ans->left != nullptr)
                ans = ans->left;
        }
        else
        {
            AVLnode* cur = ans->parent;

            while (cur != nullptr && ans == cur->right)
            {
                ans = cur;
                cur = cur->parent;
            }
            ans = cur;
        }
        return ans;
    }

    AVLnode* prevNode()
    {
        AVLnode* ans = this;

        if (left != nullptr)
        {
            ans = left;
            while (ans->right != nullptr)
                ans = ans->right;
        }
        else
        {
            AVLnode* cur = ans->parent;

            while (cur != nullptr && ans == cur->left)
            {
                ans = cur;
                cur = cur->parent;
            }
            ans = cur;
        }
        return ans;
    }
};


template <class T> 
struct less { bool operator() (const T& x, const T& y) const { return x < y; } };

template <typename T>
int comp(T a, T b)
{
    if (a < b)
        return -1;

    if (b < a)
        return 1;
    
    return 0;
}

template <class T>
class AVLTree
{
public:
    AVLTree(void) { }
    ~AVLTree(void) { }

private:
    AVLnode<T>* rotateLeft(AVLnode<T>* a)
    {
        if (a == nullptr) 
            return a;
        AVLnode<T>* b = a->right;
        a->right = b->left;
        b->left = a;

        update(a);
        update(b);

        return b;
    }

    AVLnode<T>* rotateRight(AVLnode<T>* a)
    {
        if (a == nullptr)
            return a;

        AVLnode<T>* b = a->left;
        a->left = b->right;
        b->right = a;

        update(a);
        update(b);

        return b;
    }

    AVLnode<T>* rotateLeftThenRight(AVLnode<T>* cur)
    {
        cur->left = rotateLeft(cur->left);
        return rotateRight(cur);
    }

    AVLnode<T>* rotateRightThenLeft(AVLnode<T>* cur)
    {
        cur->right = rotateRight(cur->right);
        return rotateLeft(cur);
    }

    int height(AVLnode<T>* cur)
    {
        if (cur == nullptr)
            return 0;
        return cur->height;
    }

    void update(AVLnode<T>* cur)
    {
        if (cur == nullptr)
            return;

        if (cur->left)
            cur->left->parent = cur;
        
        if (cur->right)
            cur->right->parent = cur;
        
        cur->height = std::max(height(cur->left), height(cur->right)) + 1;
        cur->balance = height(cur->right) - height(cur->left);
    }

    AVLnode<T>* rebalance(AVLnode<T>* cur)
    {
        if (cur == nullptr)
            return nullptr;

        update(cur);

        if (cur->balance == -2)
        {
            if (cur->left->balance < 0)
                cur = rotateRight(cur);
            else
                cur = rotateLeftThenRight(cur);
        }
        else if (cur->balance == 2)
        {
            if (cur->right->balance > 0)
                cur = rotateLeft(cur);
            else
                cur = rotateRightThenLeft(cur);
        }
        return cur;
    }

public:
    AVLnode<T>* insertNode(AVLnode<T>* cur, const T& key)
    {
        if (cur == nullptr)
        {
            AVLnode<T>* tmp = nullptr;

            tmp = new AVLnode<T>(key);
            return tmp;
        }
        
        if (comp(cur->key, key) == 1)
            cur->left = insertNode(cur->left, key);
        else
            cur->right = insertNode(cur->right, key);

        cur = rebalance(cur);

        return cur;
    }

    AVLnode<T>* removeMin(AVLnode<T>* cur)
    {
        if (cur->left == nullptr)
            return cur->right;

        cur->left = removeMin(cur->left);
        cur = rebalance(cur);
        
        return cur;
    }

    AVLnode<T>* removeNode(AVLnode<T>* cur, const T& key)
    {
        if (cur == nullptr)
            return cur;

        if (comp(cur->key, key) > 0) 
            cur->left = removeNode(cur->left, key);
        else if (comp(cur->key, key) < 0) 
            cur->right = removeNode(cur->right, key);
        else 
        {
            AVLnode<T>* lchild = cur->left;
            AVLnode<T>* rchild = cur->right;

            cur->makeLeaf();
            delete cur;

            if (rchild == nullptr) 
                return lchild;
            
            cur = rchild;
            while (cur->left != nullptr) 
                cur = cur->left;
            
            cur->right = removeMin(rchild);
            cur->left = lchild;
        }
        cur = rebalance(cur);
        
        return cur;
    }

    AVLnode<T>* smallest(AVLnode<T>* cur) const
    {
        while (cur->left != nullptr) 
            cur = cur->left;

        return cur;
    }

    AVLnode<T>* biggest(AVLnode<T>* cur) const
    {
        while (cur->right != nullptr) 
            cur = cur->right;

        return cur;
    }

    AVLnode<T>* begin(AVLnode<T>* root) const 
    {
        if (root == nullptr) 
            return nullptr;
    
        AVLnode<T>* tmp = smallest(root);
        
        return tmp;
    }

    AVLnode<T>* rbegin(AVLnode<T>* root) const
    {
        if (root == nullptr) 
            return nullptr;

        AVLnode<T>* tmp = biggest(root);

        return tmp;
    }

    AVLnode<T>* lower_bound(AVLnode<T>* cur, const T& key) const
    {
        if (cur == nullptr) 
            return nullptr;

        if (comp(cur->key, key) == 1) 
        {
            AVLnode<T>* tmp = lower_bound(cur->left, key);
            if (tmp == nullptr) 
                return cur;
            else 
                return tmp;
        }
        else if (comp(cur->key, key) == -1) 
            return lower_bound(cur->right, key);

        return cur;
    }

    AVLnode<T>* find(AVLnode<T>* cur, const T& key) const
    {
        AVLnode<T>* tmp = lower_bound(cur, key);

        if (tmp == nullptr || comp(tmp->key, key) != 0) 
            return nullptr;
        
        return tmp;
    }

    void outNode(AVLnode<T>* node)
    {
        if (node == nullptr) 
            std::cout << "none";
        else 
            std::cout << node->key;
    }

    void printAll(AVLnode<T>* n, bool verbose)
    {
        if (n == nullptr)
            return;

        printAll(n->left, verbose);
        std::cout << n->key;
        /*
                if (verbose)
                {
                    std::cout << "(parent=";
                    outNode(n->parent);
                    std::cout << ")";
                }
                std::cout << " ";
        */
        printAll(n->right, verbose);
    }
};

