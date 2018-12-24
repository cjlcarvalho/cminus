#pragma once
#include <cminus/ast-visitor.hpp>

namespace cminus
{
/// This is a code generator for MIPS.
///
/// The generated code is fully compatible with the O32 ABI, thus functions
/// generated by this may be used by foreign functions in the system.
///
/// This generator does no perform register allocation, therefore the
/// spit code makes very poor use of registers. Indeed, it makes poor
/// use of everything as there is no optimization whatsover.
///
class ASTCodegenVisitor : public ASTVisitor
{
public:
    explicit ASTCodegenVisitor(std::string& dest) :
        dest(dest)
    {
    }

    void visit_program(ASTProgram& program) override;

    void visit_var_decl(ASTVarDecl& decl) override;
    void visit_parm_decl(ASTParmVarDecl& decl) override;
    void visit_fun_decl(ASTFunDecl& decl) override;

    void visit_null_stmt(ASTNullStmt& stmt) override;
    void visit_compound_stmt(ASTCompoundStmt& stmt) override;
    void visit_selection_stmt(ASTSelectionStmt& stmt) override;
    void visit_iteration_stmt(ASTIterationStmt& stmt) override;
    void visit_return_stmt(ASTReturnStmt& stmt) override;

    void visit_number_expr(ASTNumber& expr) override;
    void visit_var_expr(ASTVarRef& expr) override;
    void visit_call_expr(ASTFunCall& expr) override;
    void visit_binary_expr(ASTBinaryExpr& expr) override;

    void visit_type(ExprType type) override;
    void visit_name(SourceRange name) override;

public:
    struct FrameInfo
    {
        // $sp => | output | temp | saved | local | input |
        int32_t input_size = 0;
        int32_t local_size = 0;
        int32_t saved_size = 0;
        int32_t temp_size = 0;
        int32_t output_size = 0;

        uint32_t total_size() const;

        int32_t output_offset(int32_t offset) const;
        int32_t temp_offset(int32_t offset) const;
        int32_t saved_offset(int32_t offset) const;
        int32_t local_offset(int32_t offset) const;
        int32_t input_offset(int32_t offset) const;
    };

private:
    /// Loads the address of the variable into $v0.
    void load_address_of(ASTVarRef&);

    /// Emits a store word into the current stack frame.
    void emit_frame_sw(int reg, int32_t frame_offset);

    /// Emits a load word from the current stack frame.
    void emit_frame_lw(int reg, int32_t frame_offset);

    /// Allocates temporary space in the stack frame.
    ///
    /// This size must have been calculated by `FrameAllocator`. Otherwise,
    /// bad things may happen.
    int32_t temp_alloc(int32_t size);

    /// Frees temporary space from the stack frame.
    void temp_free(int32_t offset, int32_t size);

    /// Generates a label id.
    int32_t next_label_id();

    /// Gets the name of the register number `reg`.
    auto regname(int reg) -> const char*;

private:
    std::string& dest;
    std::unordered_map<ASTFunDecl*, FrameInfo> frames;
    std::unordered_map<ASTVarDecl*, int32_t> local_pos;

    FrameInfo current_frame;
    int32_t current_temp_pos = 0;
    int32_t current_label_id = 0;
    bool inside_function = false;
    int32_t function_label_goto_ob = -1;
    int32_t function_epilogue_label;
};
}
