
namespace lobster
{
    
struct Optimizer
{
    Parser &parser;
    SymbolTable &st;
    bool changes_this_pass;
    size_t total_changes;
    Value constant;
    Node *dummy_node;
    
    Optimizer(Parser &_p, SymbolTable &_st, int maxpasses)
        : parser(_p), st(_st), changes_this_pass(true), total_changes(0), dummy_node(nullptr)
    {
        //dummy_node = NewNode(T_EMPTY, type_any);
        int i = 0;
        for (; changes_this_pass && i < maxpasses; i++)
        {
            changes_this_pass = false;
            Optimize(parser.root);
            for (auto f : parser.st.functiontable)
            {
                if (f->subf && f->subf->typechecked)
                {
                    for (auto sf = f->subf; sf; sf = sf->next) if (sf->body) Optimize(sf->body);
                }
            }
        }
        Output(OUTPUT_INFO, "optimizer: %d passes, %d optimizations", i, total_changes);
        assert(i);  // Must run at least one pass.
    }
    
    void Changed() { changes_this_pass = true; total_changes++; }
    
    Node *NewNode(Node *context, TType t, TypeRef type, Node *a = nullptr, Node *b = nullptr)
    {
        auto n = new Node(context->line, t, a, b);
        n->exptype = type;
        return n;
    }
    
    Node *NewTernary(Node *context, TType t, TypeRef type, Node *a = nullptr, Node *b = nullptr, Node *c = nullptr)
    {
        auto n = (Node *)new Ternary(context->line, t, a, b, c);
        n->exptype = type;
        return n;
    }

    void Optimize(Node *&n_ptr)
    {
        Node &n = *n_ptr;

        switch (n.type)
        {
            case T_LIST:
                // Flatten the Optimize recursion a bit
                for (Node *stats = &n; stats; stats = stats->b()) Optimize(stats->aref());
                return;
                
            case T_IF:
            {
                Optimize(n.if_condition());
                auto cv = n.if_condition()->ConstVal();
                if (cv.type != V_UNDEFINED)
                {
                    Changed();
                    auto branch = constant.True() ? n.if_then() : n.if_else();
                    auto other  = constant.True() ? n.if_else() : n.if_then();
                    Optimize(branch);
                    if (branch->type != T_DEFAULTVAL)
                    {
                        n_ptr = NewTernary(branch, T_DYNCALL, branch->sf()->returntypes[0], branch, branch, nullptr);
                    }
                    else
                    {
                        n_ptr = branch;
                    }
                    if (!other->sf()->typechecked)
                    {
                        // Typechecker did not typecheck this function for use in this if-then, but neither did any
                        // other instances, so it can be removed.
                        other->sf()->parent->DeleteSubFunction(other->sf());
                    }
                }
                else
                {
                    Optimize(n.if_then());
                    Optimize(n.if_else());
                }
                return;
            }
        }
        
        if (n.a()) Optimize(n.aref());
        if (n.b()) Optimize(n.bref());
        if (n.c()) Optimize(n.cref());
        
        switch (n.type)
        {
        }
    }
};
    
}  // namespace lobster

