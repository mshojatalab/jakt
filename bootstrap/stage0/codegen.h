#pragma once
#include "__unified_forward.h"
#include "parser.h"
#include "typechecker.h"
#include "types.h"
#include "utility.h"
#include "compiler.h"
namespace Jakt {
namespace codegen {
namespace AllowedControlExits_Details {
struct Nothing {
};
struct JustReturn {
};
struct AtLoop {
};
}
struct AllowedControlExits : public Variant<AllowedControlExits_Details::Nothing, AllowedControlExits_Details::JustReturn, AllowedControlExits_Details::AtLoop> {
using Variant<AllowedControlExits_Details::Nothing, AllowedControlExits_Details::JustReturn, AllowedControlExits_Details::AtLoop>::Variant;
    using Nothing = AllowedControlExits_Details::Nothing;
    using JustReturn = AllowedControlExits_Details::JustReturn;
    using AtLoop = AllowedControlExits_Details::AtLoop;
ErrorOr<DeprecatedString> debug_description() const;
codegen::AllowedControlExits allow_return() const;
};
struct ControlFlowState {
  public:
codegen::AllowedControlExits allowed_exits;bool passes_through_match;bool passes_through_try;size_t match_nest_level;codegen::ControlFlowState enter_function() const;
ErrorOr<DeprecatedString> apply_control_flow_macro(DeprecatedString const x) const;
codegen::ControlFlowState enter_loop() const;
static codegen::ControlFlowState no_control_flow();
bool is_match_nested() const;
codegen::ControlFlowState enter_match() const;
ControlFlowState(codegen::AllowedControlExits a_allowed_exits, bool a_passes_through_match, bool a_passes_through_try, size_t a_match_nest_level);

ErrorOr<DeprecatedString> debug_description() const;
};struct CodegenDebugInfo {
  public:
NonnullRefPtr<compiler::Compiler> compiler;JaktInternal::Dictionary<size_t,JaktInternal::DynamicArray<codegen::LineSpan>> line_spans;bool statement_span_comments;ErrorOr<void> gather_line_spans();
ErrorOr<DeprecatedString> span_to_source_location(utility::Span const span);
CodegenDebugInfo(NonnullRefPtr<compiler::Compiler> a_compiler, JaktInternal::Dictionary<size_t,JaktInternal::DynamicArray<codegen::LineSpan>> a_line_spans, bool a_statement_span_comments);

ErrorOr<DeprecatedString> debug_description() const;
};struct CodeGenerator {
  public:
NonnullRefPtr<compiler::Compiler> compiler;NonnullRefPtr<types::CheckedProgram> program;codegen::ControlFlowState control_flow_state;JaktInternal::DynamicArray<JaktInternal::Tuple<DeprecatedString,DeprecatedString>> entered_yieldable_blocks;DeprecatedString deferred_output;JaktInternal::Optional<NonnullRefPtr<types::CheckedFunction>> current_function;bool inside_defer;codegen::CodegenDebugInfo debug_info;JaktInternal::DynamicArray<DeprecatedString> namespace_stack;size_t fresh_var_counter;size_t fresh_label_counter;JaktInternal::Optional<DeprecatedString> this_replacement;static ErrorOr<JaktInternal::Dictionary<DeprecatedString,JaktInternal::Tuple<DeprecatedString,DeprecatedString>>> generate(NonnullRefPtr<compiler::Compiler> const compiler, NonnullRefPtr<types::CheckedProgram> const program, bool const debug_info);
ErrorOr<DeprecatedString> codegen_destructor(types::CheckedStruct const& struct_, NonnullRefPtr<types::CheckedFunction> const& function, bool const is_inline);
ErrorOr<DeprecatedString> codegen_unchecked_binary_op(NonnullRefPtr<typename types::CheckedExpression> const lhs, NonnullRefPtr<typename types::CheckedExpression> const rhs, parser::BinaryOperator const op, ids::TypeId const type_id);
ErrorOr<DeprecatedString> codegen_namespace_qualifier(ids::ScopeId const scope_id, bool const skip_current, JaktInternal::Optional<DeprecatedString> const possible_constructor_name) const;
ErrorOr<DeprecatedString> codegen_checked_binary_op(NonnullRefPtr<typename types::CheckedExpression> const lhs, NonnullRefPtr<typename types::CheckedExpression> const rhs, parser::BinaryOperator const op, ids::TypeId const type_id);
ErrorOr<DeprecatedString> codegen_constructor_predecl(NonnullRefPtr<types::CheckedFunction> const function);
bool is_full_respecialization(JaktInternal::DynamicArray<ids::TypeId> const type_args) const;
ErrorOr<JaktInternal::Dictionary<ids::TypeId,JaktInternal::DynamicArray<ids::TypeId>>> produce_codegen_dependency_graph(NonnullRefPtr<types::Scope> const scope) const;
ErrorOr<DeprecatedString> codegen_statement(NonnullRefPtr<typename types::CheckedStatement> const statement);
ErrorOr<DeprecatedString> codegen_function_generic_parameters(NonnullRefPtr<types::CheckedFunction> const function);
ErrorOr<DeprecatedString> codegen_function_predecl(NonnullRefPtr<types::CheckedFunction> const function, bool const as_method, bool const allow_generics);
ErrorOr<DeprecatedString> codegen_match_body(types::CheckedMatchBody const body, ids::TypeId const return_type_id);
ErrorOr<DeprecatedString> codegen_ak_formatter(DeprecatedString const name, JaktInternal::DynamicArray<DeprecatedString> const generic_parameter_names);
ErrorOr<DeprecatedString> codegen_method_call(NonnullRefPtr<typename types::CheckedExpression> const expr, types::CheckedCall const call, bool const is_optional);
ErrorOr<DeprecatedString> fresh_label();
ErrorOr<DeprecatedString> fresh_var();
ErrorOr<DeprecatedString> codegen_call(types::CheckedCall const call);
ErrorOr<DeprecatedString> codegen_namespace_predecl(NonnullRefPtr<types::Scope> const scope, NonnullRefPtr<types::Module> const current_module);
ErrorOr<JaktInternal::DynamicArray<ids::ModuleId>> topologically_sort_modules() const;
ErrorOr<DeprecatedString> codegen_block(types::CheckedBlock const block);
ErrorOr<DeprecatedString> codegen_generic_match(NonnullRefPtr<typename types::CheckedExpression> const expr, JaktInternal::DynamicArray<types::CheckedMatchCase> const cases, ids::TypeId const return_type_id, bool const all_variants_constant);
ErrorOr<DeprecatedString> codegen_debug_description_getter(types::CheckedStruct const struct_, bool const is_inline);
ErrorOr<DeprecatedString> codegen_lambda_block(bool const can_throw, types::CheckedBlock const block, ids::TypeId const return_type_id);
ErrorOr<DeprecatedString> codegen_match(NonnullRefPtr<typename types::CheckedExpression> const expr, JaktInternal::DynamicArray<types::CheckedMatchCase> const match_cases, ids::TypeId const type_id, bool const all_variants_constant);
ErrorOr<DeprecatedString> codegen_namespace_specializations(NonnullRefPtr<types::Scope> const scope, NonnullRefPtr<types::Module> const current_module);
ErrorOr<DeprecatedString> codegen_enum_type(ids::EnumId const id, bool const as_namespace) const;
ErrorOr<JaktInternal::DynamicArray<ids::TypeId>> extract_dependencies_from_struct(ids::StructId const struct_id, JaktInternal::Dictionary<ids::TypeId,JaktInternal::DynamicArray<ids::TypeId>> const dependency_graph, bool const top_level, JaktInternal::DynamicArray<ids::TypeId> const args) const;
ErrorOr<DeprecatedString> codegen_checked_binary_op_assignment(NonnullRefPtr<typename types::CheckedExpression> const lhs, NonnullRefPtr<typename types::CheckedExpression> const rhs, parser::BinaryOperator const op, ids::TypeId const type_id);
ErrorOr<DeprecatedString> codegen_expression_and_deref_if_generic_and_needed(NonnullRefPtr<typename types::CheckedExpression> const expression);
ErrorOr<DeprecatedString> codegen_struct(types::CheckedStruct const struct_);
CodeGenerator(NonnullRefPtr<compiler::Compiler> a_compiler, NonnullRefPtr<types::CheckedProgram> a_program, codegen::ControlFlowState a_control_flow_state, JaktInternal::DynamicArray<JaktInternal::Tuple<DeprecatedString,DeprecatedString>> a_entered_yieldable_blocks, DeprecatedString a_deferred_output, JaktInternal::Optional<NonnullRefPtr<types::CheckedFunction>> a_current_function, bool a_inside_defer, codegen::CodegenDebugInfo a_debug_info, JaktInternal::DynamicArray<DeprecatedString> a_namespace_stack, size_t a_fresh_var_counter, size_t a_fresh_label_counter, JaktInternal::Optional<DeprecatedString> a_this_replacement);

ErrorOr<DeprecatedString> codegen_function(NonnullRefPtr<types::CheckedFunction> const function, bool const as_method);
ErrorOr<DeprecatedString> codegen_generic_type_instance(ids::StructId const id, JaktInternal::DynamicArray<ids::TypeId> const args, bool const as_namespace) const;
ErrorOr<DeprecatedString> codegen_enum_debug_description_getter(types::CheckedEnum const enum_, bool const is_inline);
ErrorOr<DeprecatedString> codegen_expression(NonnullRefPtr<typename types::CheckedExpression> const expression);
ErrorOr<DeprecatedString> codegen_binary_expression(NonnullRefPtr<typename types::CheckedExpression> const expression, ids::TypeId const type_id, NonnullRefPtr<typename types::CheckedExpression> const lhs, NonnullRefPtr<typename types::CheckedExpression> const rhs, types::CheckedBinaryOperator const op);
ErrorOr<DeprecatedString> current_error_handler() const;
ErrorOr<DeprecatedString> codegen_enum_match(types::CheckedEnum const enum_, NonnullRefPtr<typename types::CheckedExpression> const expr, JaktInternal::DynamicArray<types::CheckedMatchCase> const match_cases, ids::TypeId const type_id, bool const all_variants_constant);
ErrorOr<DeprecatedString> codegen_namespace_path(types::CheckedCall const call) const;
ErrorOr<DeprecatedString> codegen_enum(types::CheckedEnum const enum_);
ErrorOr<DeprecatedString> codegen_struct_type(ids::StructId const id, bool const as_namespace) const;
ErrorOr<DeprecatedString> codegen_enum_predecl(types::CheckedEnum const enum_);
ErrorOr<void> postorder_traversal(ids::TypeId const type_id, JaktInternal::Set<ids::TypeId> visited, JaktInternal::Dictionary<ids::TypeId,JaktInternal::DynamicArray<ids::TypeId>> const dependency_graph, JaktInternal::DynamicArray<ids::TypeId> output) const;
ErrorOr<JaktInternal::DynamicArray<ids::TypeId>> extract_dependencies_from_enum(ids::EnumId const enum_id, JaktInternal::Dictionary<ids::TypeId,JaktInternal::DynamicArray<ids::TypeId>> const dependency_graph, bool const top_level) const;
ErrorOr<DeprecatedString> codegen_generic_enum_instance(ids::EnumId const id, JaktInternal::DynamicArray<ids::TypeId> const args, bool const as_namespace) const;
ErrorOr<DeprecatedString> codegen_function_return_type(NonnullRefPtr<types::CheckedFunction> const function);
ErrorOr<DeprecatedString> codegen_function_in_namespace(NonnullRefPtr<types::CheckedFunction> const function, JaktInternal::Optional<ids::TypeId> const containing_struct, bool const as_method, bool const skip_template, JaktInternal::Optional<JaktInternal::DynamicArray<ids::TypeId>> const explicit_generic_instantiation);
ErrorOr<DeprecatedString> codegen_type(ids::TypeId const type_id) const;
ErrorOr<JaktInternal::Optional<DeprecatedString>> codegen_namespace_specialization(ids::FunctionId const function_id, JaktInternal::Optional<ids::TypeId> const containing_struct, NonnullRefPtr<types::Scope> const scope, NonnullRefPtr<types::Module> const current_module, bool const define_pass);
ErrorOr<DeprecatedString> codegen_destructor_predecl(types::CheckedStruct const& struct_);
ErrorOr<JaktInternal::DynamicArray<ids::TypeId>> extract_dependencies_from(ids::TypeId const type_id, JaktInternal::Dictionary<ids::TypeId,JaktInternal::DynamicArray<ids::TypeId>> const dependency_graph, bool const top_level) const;
ErrorOr<DeprecatedString> codegen_namespace(NonnullRefPtr<types::Scope> const scope, NonnullRefPtr<types::Module> const current_module, bool const as_forward);
ErrorOr<DeprecatedString> codegen_unchecked_binary_op_assignment(NonnullRefPtr<typename types::CheckedExpression> const lhs, NonnullRefPtr<typename types::CheckedExpression> const rhs, parser::BinaryOperator const op, ids::TypeId const type_id);
ErrorOr<DeprecatedString> codegen_constructor(NonnullRefPtr<types::CheckedFunction> const function, bool const is_inline);
ErrorOr<DeprecatedString> codegen_type_possibly_as_namespace(ids::TypeId const type_id, bool const as_namespace) const;
ErrorOr<DeprecatedString> codegen_struct_predecl(types::CheckedStruct const struct_);
ErrorOr<DeprecatedString> debug_description() const;
};struct LineSpan {
  public:
size_t start;size_t end;LineSpan(size_t a_start, size_t a_end);

ErrorOr<DeprecatedString> debug_description() const;
};}
} // namespace Jakt
template<>struct Jakt::Formatter<Jakt::codegen::AllowedControlExits> : Jakt::Formatter<Jakt::StringView>{
Jakt::ErrorOr<void> format(Jakt::FormatBuilder& builder, Jakt::codegen::AllowedControlExits const& value) {
JaktInternal::PrettyPrint::ScopedEnable pretty_print_enable { m_alternative_form };Jakt::ErrorOr<void> format_error = Jakt::Formatter<Jakt::StringView>::format(builder, MUST(value.debug_description()));return format_error;}
};
namespace Jakt {
} // namespace Jakt
template<>struct Jakt::Formatter<Jakt::codegen::ControlFlowState> : Jakt::Formatter<Jakt::StringView>{
Jakt::ErrorOr<void> format(Jakt::FormatBuilder& builder, Jakt::codegen::ControlFlowState const& value) {
JaktInternal::PrettyPrint::ScopedEnable pretty_print_enable { m_alternative_form };Jakt::ErrorOr<void> format_error = Jakt::Formatter<Jakt::StringView>::format(builder, MUST(value.debug_description()));return format_error;}
};
namespace Jakt {
} // namespace Jakt
template<>struct Jakt::Formatter<Jakt::codegen::CodegenDebugInfo> : Jakt::Formatter<Jakt::StringView>{
Jakt::ErrorOr<void> format(Jakt::FormatBuilder& builder, Jakt::codegen::CodegenDebugInfo const& value) {
JaktInternal::PrettyPrint::ScopedEnable pretty_print_enable { m_alternative_form };Jakt::ErrorOr<void> format_error = Jakt::Formatter<Jakt::StringView>::format(builder, MUST(value.debug_description()));return format_error;}
};
namespace Jakt {
} // namespace Jakt
template<>struct Jakt::Formatter<Jakt::codegen::CodeGenerator> : Jakt::Formatter<Jakt::StringView>{
Jakt::ErrorOr<void> format(Jakt::FormatBuilder& builder, Jakt::codegen::CodeGenerator const& value) {
JaktInternal::PrettyPrint::ScopedEnable pretty_print_enable { m_alternative_form };Jakt::ErrorOr<void> format_error = Jakt::Formatter<Jakt::StringView>::format(builder, MUST(value.debug_description()));return format_error;}
};
namespace Jakt {
} // namespace Jakt
template<>struct Jakt::Formatter<Jakt::codegen::LineSpan> : Jakt::Formatter<Jakt::StringView>{
Jakt::ErrorOr<void> format(Jakt::FormatBuilder& builder, Jakt::codegen::LineSpan const& value) {
JaktInternal::PrettyPrint::ScopedEnable pretty_print_enable { m_alternative_form };Jakt::ErrorOr<void> format_error = Jakt::Formatter<Jakt::StringView>::format(builder, MUST(value.debug_description()));return format_error;}
};
namespace Jakt {
} // namespace Jakt
