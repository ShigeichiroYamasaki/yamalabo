# Move言語


## Encoding Digital Assets in an Open System


 
## 検証可能性

理想的には、Moveプログラムのすべての安全特性をオンチェーンバイトコード分析またはランタイムで検証できるべきです。しかし残念ながら、それは現実的ではありません。

オンチェーン検証に対する、計算コストに対する安全性保証、一般性、追加されたプロトコルの複雑さの重要性を慎重に検討しなければなりません。

1. ダイナミックディスパッチの禁止

The target of each call site can be statically determined. This makes
it easy for verification tools to reason precisely about the effects of a procedure call without
performing a complex call graph construction analysis.

2. ミュータビリティの制限

Every mutation to a Move value occurs through a reference. References
are temporary values that must be created and destroyed within the confines of a single transaction script. Move’s bytecode verifier uses a “borrow checking” scheme similar to Rust to ensure
that at most one mutable reference to a value exists at any point in time. In addition, the
language ensures that global storage is always a tree instead of an arbitrary graph. This allows
verification tools to modularize reasoning about the effects of a write operation.

3. モジュラリティ

Move modules enforce data abstraction and localize critical operations on resources. The encapsulation enabled by a module combined with the protections enforced by
the Move type system ensures that the properties established for a module’s types cannot be
violated by code outside the module. We expect this design to enable exhaustive functional verification of important mod


## 言語仕様

###  Peer-to-Peer Payment Transaction Script

```
public main(payee: address, amount: u64) {
    let coin: 0x0.Currency.Coin = 0x0.Currency.withdraw_from_sender(copy(amount));
    0x0.Currency.deposit(copy(payee), move(coin));
}
```

This script takes two inputs: the account address of the payment’s recipient and an unsigned integer that represents the number of coins to be transferred to the recipient. 

The effect of executing this
script is straightforward: amount coins will be transferred from the transaction sender to payee. 

This happens in two steps. 

In the first step, the sender invokes a procedure named *withdraw\_from\_sender*
from the module stored at 0x0.Currency.

 As we will explain in Section 4.2, 0x0 is the account address4 where the module is stored and Currency is the name of the module. The value coin returned by this procedure is a resource value whose type is 0x0.Currency.Coin. 

In the second step,
the sender transfers the funds to payee by moving the coin resource value into the 0x0.Currency module’s deposit procedure.

This example is interesting because it is quite delicate. 
Move’s type system will reject small variants of the same code that would lead to bad behavior. In particular, the type system ensures that resources can never be duplicated, reused, or lost. For example, the following three changes to the script would be rejected by the type system:

#### Duplicating currency by changing move(coin) to copy(coin). 

Note that each usage of a
variable in the example is wrapped in either copy() or move(). Move, following Rust and C++,
implements move semantics. Each read of a Move variable x must specify whether the usage moves
x’s value out of the variable (rendering x unavailable) or copies the value (leaving x available for
continued use). Unrestricted values like u64 and address can be both copied and moved. But
resource values can only be moved. Attempting to duplicate a resource value (e.g., using copy(coin)
in the example above) will cause an error at bytecode verification time.


###  Currency Module



このセクションでは、Move言語、バイトコード検証、および
仮想マシンについて述べる

付録Aでは、これらすべてのコンポーネントを詳細に説明していますが、散文は付属していません。

ここでの議論では、付録の抜粋を使用し、時々参照します
そこで定義されたシンボル。



## 付録A　 Move Language Reference

In this appendix, we present the structure of programs and state in the Move bytecode language.


### Identifiers

```
𝑛 ∈ StructName
𝑓 ∈ FieldName
𝑥 ∈ VarName
ProcedureName
ModuleName
```

### Types and Kinds

```
𝑎 ∈ AccountAddress
𝑏 ∈ Bool
𝑢 ∈ UnsignedInt64
⃗𝑏 ∈ Bytes
Kind ::= resource | unrestricted
ModuleID = AccountAddress × ModuleName
StructID = ModuleID × StructName
StructType = StructID × Kind
PrimitiveType = AccountAddress ∪ Bool ∪ UnsignedInt64 ∪ Bytes
𝒯 ⊆ NonReferenceType = StructType ∪ PrimitiveType
Type ::= 𝒯 | &mut 𝒯 | & 𝒯
```

### Values

```
Resource = FieldName ⇀ Value
Struct = FieldName ⇀ UnrestrictedValue
PrimitiveValue ::= 𝑎 | 𝑏 | 𝑢 |
⃗𝑏
UnrestrictedValue = Struct ∪ PrimitiveValue
𝑣 ∈ Value = Resource ∪ UnrestrictedValue
𝑔 ∈ GlobalResourceKey = AccountAddress × StructID
𝑎𝑝 ∈ AccessPath ::= 𝑥 | 𝑔 | 𝑎𝑝 . 𝑓
𝑟 ∈ RuntimeValue ::= 𝑣 | ref 𝑎𝑝
```

### Global State

```
Σ ∈ GlobalState = AccountAddress ⇀ Account
Account = (StructID ⇀ Resource) × (ModuleName ⇀ Module)
```

### Modules and Transaction Scripts

```
Module = ModuleName × (StructName ⇀ StructDecl)
× (ProcedureName ⇀ ProcedureDecl)
TransactionScript = ProcedureDecl
StructDecl = Kind × (FieldName ⇀ NonReferenceType)
ProcedureSig = Visibility × (VarName ⇀ Type) × Type∗
ProcedureDecl = ProcedureSig × (VarName ⇀ Type) × [Instrℓ
]
ℓ=𝑖
ℓ=0
Visibility ::= public | internal
ℓ ∈ InstrIndex = UnsignedInt64
```

### Interpreter State

```
𝜎 ∈ InterpreterState = ValueStack × CallStack × GlobalRefCount × GasUnits
𝑣𝑠𝑡𝑘 ∈ ValueStack ::= [] | 𝑟 :: 𝑣𝑠𝑡𝑘
𝑐𝑠𝑡𝑘 ∈ CallStack ::= [] | 𝑐 :: 𝑐𝑠𝑡𝑘
𝑐 ∈ CallStackFrame = Locals × ProcedureID × InstrIndex
Locals = VarName ⇀ RuntimeValue
𝑝 ∈ ProcedureID = ModuleID × ProcedureSig
GlobalRefCount = GlobalResourceKey ⇀ UnsignedInt64
GasUnits = UnsignedInt64
```

### Evaluation

```
𝑇 ∈ Transaction = TransactionScript × PrimitiveValue∗ × Module∗
×AccountAddress × GasUnits …
𝐵 ∈ Block = Transaction∗ × …
𝐸 ∈ TransactionEffect = AccountAddress ⇀ Account
apply ∈ (GlobalState × TransactionEffect) updating global state
→ GlobalState
⟨𝐵, Σ⟩ ⇓ 𝐸            block evaluation
⟨𝑇 , 𝐸, Σ⟩ ⇓ 𝐸′      transaction evaluation
⟨𝜎, 𝐸, Σ⟩ ⇓ 𝜎′, 𝐸′  interpreter state evaluation
```

### Verification

```
𝐶 ∈ Code = TransactionScript ∪ Module
𝑧 ∈ VerificationResult ::= ok | stack_err | type_err | reference_err | …
𝐶 ⇝ 𝑧 bytecode verification
𝐷 ∈ Dependencies = StructType∗ × ProcedureID∗
deps ∈ Code → Dependencies computing dependencies
𝑙 ∈ LinkingResult ::= success | fail
⟨𝐷, Σ⟩ ↪ 𝑙 linking dependencies with global state
```

### Instructions † indicates an instruction whose execution may fail at runtime

```
LocalInstr ::=
MoveLoc< 𝑥 > Push value stored in 𝑥 on the stack. 𝑥 is now unavailable.
| CopyLoc< 𝑥 > Push value stored in 𝑥 on the stack.
| StoreLoc< 𝑥 > Pop the stack and store the result in 𝑥. 𝑥 is now available.
| BorrowLoc< 𝑥 > Create a reference to the value stored in 𝑥 and push it on the stack.
ReferenceInstr ::=
ReadRef Pop 𝑟 and push ∗𝑟 on the stack.
| WriteRef Pop two values 𝑣 and 𝑟, perform the write ∗𝑟 = 𝑣.
| ReleaseRef Pop 𝑟 and decrement the appropriate refcount if 𝑟 is a global reference.
| FreezeRef Pop mutable reference 𝑟, push immutable reference to the same value.
CallInstr ::=
Call< 𝑝 > Pop arguments 𝑟
∗
, load into 𝑝’s formals 𝑥
∗
, transfer control to 𝑝.
| Return Return control to the previous frame in the call stack.
ModuleBuiltinInstr ::=
Pack< 𝑛 > Pop arguments 𝑣
∗
, create struct of type 𝑛 with 𝑓𝑖
: 𝑣𝑖
, push it on the stack.
| Unpack< 𝑛 > Pop struct of type 𝑛 from the stack and push its field values 𝑣
∗ on the stack.
| BorrowField< 𝑓 > Pop reference to a struct and push a reference to field 𝑓 of the struct.
| MoveToSender< 𝑛 >† Pop resource of type 𝑛 and publish it under the sender’s address.
| MoveFrom< 𝑛 >† Pop address 𝑎, remove resource of type 𝑛 from 𝑎, push it.
| BorrowGlobal< 𝑛 >† Pop address 𝑎, push a reference to the resource of type 𝑛 under 𝑎.
| Exists< 𝑛 > Pop address 𝑎, push bool encoding “a resource of type 𝑛 exists under 𝑎”.
TxnBuiltinInstr ::=
GetGasRemaining Push u64 representing remaining gas unit budget.
| GetTxnSequenceNumber Push u64 encoding the transaction’s sequence number.
| GetTxnPublicKey Push byte array encoding the transaction sender’s public key.
| GetTxnSenderAddress Push address encoding the sender of the transaction.
| GetTxnMaxGasUnits Push u64 representing the initial gas unit budget.
| GetTxnGasUnitPrice Push u64 representing the Libra coin per gas unit price.
SpecialInstr ::=
PopUnrestricted Pop a non-resource value.
| RejectUnless† Pop bool 𝑏 and u64 𝑢, fail with error code 𝑢 if 𝑏 is false.
| CreateAccount† Pop address 𝑎, create a LibraAccount.T under 𝑎.
ConstantInstr ::=
LoadTrue Push true on the stack.
| LoadFalse Push false on the stack.
| LoadU64< 𝑢 > Push the u64 𝑢 on the stack.
| LoadAddress< 𝑎 > Push the address 𝑎 on the stack.
| LoadBytes< ⃗𝑏 > Push the byte array ⃗𝑏 on the stack.
StackInstr ::=
Not Pop boolean 𝑏 and push ¬𝑏.
| Add† Pop two u64’s 𝑢1 and 𝑢2 and push 𝑢1 + 𝑢2
. Fail on overflow.
| Sub† Pop two u64’s 𝑢1 and 𝑢2 and push 𝑢1
- 𝑢2
. Fail on underflow.
| Mul† Pop two u64’s 𝑢1 and 𝑢2 and push 𝑢1 × 𝑢2
. Fail on overflow.
| Div† Pop two u64’s 𝑢1 and 𝑢2 and push 𝑢1 ÷ 𝑢2
. Fail if 𝑢2
is zero.
| Mod† Pop two u64’s 𝑢1 and 𝑢2 and push 𝑢1 mod 𝑢2
. Fail if 𝑢2
is zero.
| BitOr Pop two u64’s 𝑢1 and 𝑢2 and push 𝑢1
| 𝑢2
.
| BitAnd Pop two u64’s 𝑢1 and 𝑢2 and push 𝑢1 & 𝑢2
.
| Xor Pop two u64’s 𝑢1 and 𝑢2 and push 𝑢1 ⊕ 𝑢2
.
| Lt Pop two u64’s 𝑢1 and 𝑢2 and push 𝑢1 < 𝑢2
.
```

### Instructions † indicates an instruction whose execution may fail at runtime

```
| Gt Pop two u64’s 𝑢1 and 𝑢2 and push 𝑢1 > 𝑢2
.
| Le Pop two u64’s 𝑢1 and 𝑢2 and push 𝑢1 ≤ 𝑢2
.
| Ge Pop two u64’s 𝑢1 and 𝑢2 and push 𝑢1 ≥ 𝑢2
.
| Or Pop two booleans 𝑏1 and 𝑏2 and push 𝑏1 ∨ 𝑏2
.
| And Pop two booleans 𝑏1 and 𝑏2 and push 𝑏1 ∧ 𝑏2
.
| Eq Pop two values 𝑟1 and 𝑟2 and push 𝑟1 = 𝑟2
.
| Neq Pop two values 𝑟1 and 𝑟2 and push 𝑟1 ≠ 𝑟2
.
ControlFlowInstr ::=
Branch< ℓ > Jump to instruction index ℓ in the current procedure.
| BranchIfTrue< ℓ > Pop boolean, jump to instruction index ℓ in the current procedure if true.
| BranchIfFalse< ℓ > Pop boolean, jump to instruction index ℓ in the current procedure if false.
Instr =
LocalInstr
∪ ReferenceInstr
∪ CallInstr
∪ ModuleBuiltinInstr
∪ TxnBuiltinInstr
∪ SpecialInstr
∪ ConstantInstr
∪ StackInstr
∪ ControlFlowInstr
```