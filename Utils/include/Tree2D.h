#pragma once
#include <stdint.h>
#include <map>

class Tree2D {
    struct TreeNode;
public:
    struct TreeRect {
        TreeRect() {X = Y = Width = Height = 0;}
        TreeRect(uint32_t x, uint32_t y, uint32_t width, uint32_t height) 
            : X(x), Y(y), Width(width), Height(height) {}
        uint32_t X;
        uint32_t Y;
        uint32_t Width;
        uint32_t Height;
    private:
        friend class Tree2D;
        std::map<Tree2D::TreeNode*, TreeRect> _subRects;
    };

    Tree2D(uint32_t width, uint32_t height);
    ~Tree2D();

    bool FillRect(uint32_t width, uint32_t height, TreeRect& outRect);
    void FreeRect(const TreeRect& rect);

private:
    struct TreeNode : public TreeRect {
        TreeNode(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
            : TreeRect(x, y, width, height)
            , Empty(false)
            , XSeperator(x)
            , YSeperator(y)
            , Size(width * height)
            , Parent(nullptr)
            , NWChild(nullptr)
            , NEChild(nullptr)
            , SEChild(nullptr)
            , SWChild(nullptr) {}
        uint32_t    XSeperator;
        uint32_t    YSeperator;
        uint32_t    Size;
        bool        Empty;
        TreeNode*   Parent;
        TreeNode*   NWChild;
        TreeNode*   NEChild;
        TreeNode*   SWChild;
        TreeNode*   SEChild;
    };
    typedef bool (Tree2D::*Iterator_TYPE)(TreeNode* node, void* data);

    TreeNode* _getPointNode(uint32_t x, uint32_t y);
    bool _iterateInDepth(TreeNode* startNode, Iterator_TYPE iterator, void* data);
    bool _getCoveringNodes(TreeNode* root, uint32_t width, uint32_t height, std::map<Tree2D::TreeNode*, Tree2D::TreeRect>& outNodes);
    void _splitNode(TreeNode* node, const TreeRect& rect);
    void _mergeNode(TreeNode* node, const TreeRect& rect);
    bool _iterator_SearchEmptyRect(TreeNode* node, void* data);

    TreeNode*   _root;
    uint32_t    _width;
    uint32_t    _height;
};

Tree2D::Tree2D(uint32_t width, uint32_t height) 
    : _width(width)
    , _height(height) 
    , _root(new TreeNode(0, 0, width, height)){
    _root->Empty = true;
}

Tree2D::~Tree2D() {
}

bool Tree2D::FillRect(uint32_t width, uint32_t height, TreeRect& outRect) {
    outRect.X = UINT_MAX;
    outRect.Y = UINT_MAX;
    outRect.Width = width;
    outRect.Height = height;
    _iterateInDepth(_root, &Tree2D::_iterator_SearchEmptyRect, (void*)&outRect);
    if (outRect.X == UINT_MAX || outRect.Y == UINT_MAX) return false;
    for (auto iter : outRect._subRects) {
        _splitNode(iter.first, iter.second);
    }
    return true;
}

void Tree2D::FreeRect(const TreeRect& rect) {
    if (rect._subRects.empty()) {
        
    }
    for (auto iter : rect._subRects) {
        _mergeNode(iter.first, iter.second);
    }
}

Tree2D::TreeNode* Tree2D::_getPointNode(uint32_t x, uint32_t y) {
    if (x >= _width || y >= _height) {
        return nullptr;
    }
    TreeNode* cur = _root;
    TreeNode* prev = _root;
    while (cur) {
        prev = cur;
        if (x <= cur->XSeperator && y <= cur->YSeperator) {
            cur = cur->NWChild;
            continue;
        }
        if (x > cur->XSeperator && y <= cur->YSeperator) {
            cur = cur->NEChild;
            continue;
        }
        if (x <= cur->XSeperator && y > cur->YSeperator) {
            cur = cur->SWChild;
            continue;
        }
        if (x > cur->XSeperator && y > cur->YSeperator) {
            cur = cur->SEChild;
            continue;
        }
    }
    return prev;
}

bool Tree2D::_iterateInDepth(Tree2D::TreeNode* startNode, Tree2D::Iterator_TYPE iterator, void* data) {
    if (startNode) {
        if (!_iterateInDepth(startNode->NWChild, iterator, data) ||
            !_iterateInDepth(startNode->NEChild, iterator, data) ||
            !_iterateInDepth(startNode->SWChild, iterator, data) ||
            !_iterateInDepth(startNode->SEChild, iterator, data)) {
                return false;
        }

        if (!(this->*iterator)(startNode, data)) {
            return false;
        }
    }
    return true;
}

bool Tree2D::_getCoveringNodes(TreeNode* root, uint32_t width, uint32_t height, std::map<TreeNode*, TreeRect>& outNodes) {
    if (!root || root->X + width > _width || root->Y + height > _height) {
        return false;
    }
    bool nodeWidthIsEnough = root->Width >= width;
    bool nodeHeightIsEnough = root->Height >= height;
    if (nodeWidthIsEnough && nodeHeightIsEnough) {
        outNodes[root] = TreeRect(root->X, root->Y, width, height);
        return true;
    }
    else if (nodeWidthIsEnough) {
        outNodes[root] = TreeRect(root->X, root->Y, width, root->Height);
        TreeNode* SWNode = _getPointNode(root->X, root->Y + root->Height);
        _getCoveringNodes(SWNode, width, height - root->Height, outNodes);
    }
    else if (nodeHeightIsEnough) {
        outNodes[root] = TreeRect(root->X, root->Y, root->Width, height);
        TreeNode* NENode = _getPointNode(root->X + root->Width, root->Y);
        _getCoveringNodes(NENode, width - root->Width, height, outNodes);
    }
    else {
        outNodes[root] = TreeRect(root->X, root->Y,  root->Width, root->Height);
        TreeNode* SWNode = _getPointNode(root->X, root->Y + root->Height);
        _getCoveringNodes(SWNode, width, height - root->Height, outNodes);
        TreeNode* NENode = _getPointNode(root->X + root->Width, root->Y);
        _getCoveringNodes(NENode, width - root->Width, height, outNodes);
        TreeNode* SENode = _getPointNode(root->X + root->Width, root->Y + root->Height);
        _getCoveringNodes(SENode, width - root->Width, height - root->Height, outNodes);
    }
    return true;
}

void Tree2D::_splitNode(TreeNode* node, const TreeRect& rect) {
    if (node->X > rect.X || node->X + node->Width < rect.X + rect.Width ||
        node->Y > rect.Y || node->Y + node->Height < rect.Y + rect.Height) {
        throw std::runtime_error("Invalid split arguments");
    }
    if ((node->X == rect.X) && (node->Y == rect.Y) && (node->Width == rect.Width) && (node->Height == rect.Height)) {
        node->Empty = false;
        return;
    }
    uint32_t width0 = (node->Width + 1) / 2;
    uint32_t width1 = node->Width - width0;
    uint32_t height0 = (node->Height + 1) / 2;
    uint32_t height1 = node->Height - height0;
    node->XSeperator = node->X + width0;
    node->YSeperator = node->Y + height0;
    node->NWChild = new TreeNode(node->X, node->Y, width0, height0);
    node->NEChild = new TreeNode(node->XSeperator, node->Y, width1, height0);
    node->SWChild = new TreeNode(node->X, node->YSeperator, width0, height1);
    node->SEChild = new TreeNode(node->XSeperator, node->YSeperator, width1, height1);
    if      (rect.X <= node->XSeperator && rect.Y <= node->YSeperator) {
        // Rect is in NW node
        if      (rect.X + rect.Width <= node->XSeperator && rect.Y + rect.Height <= node->YSeperator) {
            _splitNode(node->NWChild, rect);
            node->NEChild->Empty = node->SWChild->Empty = node->SEChild->Empty = true;
        }
        // Rect is in NW + SW nodes
        else if (rect.X + rect.Width <= node->XSeperator && rect.Y + rect.Height > node->YSeperator) {
            TreeRect NWRect(rect.X, rect.Y, rect.Width, node->NWChild->Height - (rect.Y - node->NWChild->Y));
            TreeRect SWRect(rect.X, node->YSeperator, rect.Width, rect.Y + rect.Height - node->YSeperator);
            _splitNode(node->NWChild, NWRect);
            _splitNode(node->SWChild, SWRect);
            node->NEChild->Empty = node->SEChild->Empty = true;
        }
        // Rect is in NW + NE nodes
        else if (rect.X + rect.Width > node->XSeperator && rect.Y + rect.Height <= node->YSeperator) {
            TreeRect NWRect(rect.X, rect.Y, node->NWChild->Width - (rect.X - node->NWChild->X), rect.Height);
            TreeRect NERect(node->XSeperator, rect.Y, rect.X + rect.Width - node->XSeperator, rect.Height);
            _splitNode(node->NWChild, NWRect);
            _splitNode(node->NEChild, NERect);
            node->SEChild->Empty = node->SWChild->Empty = true;
        }
        // Rect is in NW + NE + SW + SE nodes
        else {
            TreeRect NWRect(rect.X, rect.Y, node->NWChild->Width - (rect.X - node->NWChild->X), node->NWChild->Height - (rect.Y - node->NWChild->Y));
            TreeRect NERect(node->XSeperator, rect.Y, rect.X + rect.Width - node->XSeperator, node->NWChild->Height - (rect.Y - node->NWChild->Y));
            TreeRect SWRect(rect.X, node->YSeperator, node->NWChild->Width - (rect.X - node->NWChild->X), rect.Y + rect.Height - node->YSeperator);
            TreeRect SERect(node->XSeperator, node->YSeperator, rect.X + rect.Width - node->XSeperator, rect.Y + rect.Height - node->YSeperator);
            _splitNode(node->NWChild, NWRect);
            _splitNode(node->NEChild, NERect);
            _splitNode(node->SWChild, SWRect);
            _splitNode(node->SEChild, SERect);
        }
    }
    else if (rect.X + rect.Width > node->XSeperator && rect.Y + rect.Height > node->YSeperator) {
        // Rect is in SE node
        if      (rect.X > node->XSeperator && rect.Y > node->YSeperator) {
            _splitNode(node->SEChild, rect);
            node->NEChild->Empty = node->NWChild->Empty = node->SWChild->Empty = true;
        }
        // Rect is in SE + SW nodes
        else if (rect.X <= node->XSeperator && rect.Y > node->YSeperator) {
            TreeRect SWRect(rect.X, rect.Y, node->SWChild->Width - (rect.X - node->SWChild->X), rect.Height);
            TreeRect SERect(node->XSeperator + 1, rect.Y, rect.X + rect.Width - node->XSeperator, rect.Height);
            _splitNode(node->SWChild, SWRect);
            _splitNode(node->SEChild, SERect);
            node->NWChild->Empty = node->NWChild->Empty = true;
        }
        // Rect is in SE + NE nodes
        else if (rect.X > node->XSeperator && rect.Y <= node->YSeperator) {
            TreeRect NERect(rect.X, rect.Y, rect.Width, node->NEChild->Height - (rect.Y - node->NEChild->Y));
            TreeRect SERect(rect.X, node->YSeperator + 1, rect.Width, rect.Y + rect.Height - node->YSeperator);
            _splitNode(node->NEChild, NERect);
            _splitNode(node->SEChild, SERect);
            node->NWChild->Empty = node->SWChild->Empty = true;
        }
    }
    TreeRect NWRect();
}

void Tree2D::_mergeNode(TreeNode* node, const TreeRect& rect) {
    if (node->X > rect.X || node->X + node->Width < rect.X + rect.Width ||
        node->Y > rect.Y || node->Y + node->Height < rect.Y + rect.Height) {
        throw std::runtime_error("Invalid merge arguments");
    }
}

bool Tree2D::_iterator_SearchEmptyRect(TreeNode* node, void* data) {
    if (node->Empty) {
        TreeRect* rect = (TreeRect*)data;
        std::map<TreeNode*, TreeRect> subRects;
        _getCoveringNodes(node, rect->Width, rect->Height, subRects);
        for (auto iter : subRects) {
            if (!iter.first->Empty) {
                return true; // Continue search
            }
        }
        rect->X = node->X;
        rect->Y = node->Y;
        rect->_subRects = std::move(subRects);
        return false; // Stop search, region is found
    }
    return true; // Continue search
}