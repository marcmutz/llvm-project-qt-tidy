; NOTE: Assertions have been autogenerated by utils/update_test_checks.py
; RUN: opt -S %s -atomic-expand | FileCheck %s

;; Verify the cmpxchg and atomicrmw expansions where sub-word-size
;; instructions are not available.

;;; NOTE: this test is mostly target-independent -- any target which
;;; doesn't support cmpxchg of sub-word sizes would do.
target datalayout = "E-m:e-i64:64-n32:64-S128"
target triple = "sparcv9-unknown-unknown"

define i8 @test_cmpxchg_i8(i8* %arg, i8 %old, i8 %new) {
; CHECK-LABEL: @test_cmpxchg_i8(
; CHECK-NEXT:  entry:
; CHECK-NEXT:    fence seq_cst
; CHECK-NEXT:    [[TMP0:%.*]] = ptrtoint i8* [[ARG:%.*]] to i64
; CHECK-NEXT:    [[TMP1:%.*]] = and i64 [[TMP0]], -4
; CHECK-NEXT:    [[ALIGNEDADDR:%.*]] = inttoptr i64 [[TMP1]] to i32*
; CHECK-NEXT:    [[PTRLSB:%.*]] = and i64 [[TMP0]], 3
; CHECK-NEXT:    [[TMP2:%.*]] = xor i64 [[PTRLSB]], 3
; CHECK-NEXT:    [[TMP3:%.*]] = shl i64 [[TMP2]], 3
; CHECK-NEXT:    [[SHIFTAMT:%.*]] = trunc i64 [[TMP3]] to i32
; CHECK-NEXT:    [[MASK:%.*]] = shl i32 255, [[SHIFTAMT]]
; CHECK-NEXT:    [[INV_MASK:%.*]] = xor i32 [[MASK]], -1
; CHECK-NEXT:    [[TMP4:%.*]] = zext i8 [[NEW:%.*]] to i32
; CHECK-NEXT:    [[TMP5:%.*]] = shl i32 [[TMP4]], [[SHIFTAMT]]
; CHECK-NEXT:    [[TMP6:%.*]] = zext i8 [[OLD:%.*]] to i32
; CHECK-NEXT:    [[TMP7:%.*]] = shl i32 [[TMP6]], [[SHIFTAMT]]
; CHECK-NEXT:    [[TMP8:%.*]] = load i32, i32* [[ALIGNEDADDR]], align 4
; CHECK-NEXT:    [[TMP9:%.*]] = and i32 [[TMP8]], [[INV_MASK]]
; CHECK-NEXT:    br label [[PARTWORD_CMPXCHG_LOOP:%.*]]
; CHECK:       partword.cmpxchg.loop:
; CHECK-NEXT:    [[TMP10:%.*]] = phi i32 [ [[TMP9]], [[ENTRY:%.*]] ], [ [[TMP16:%.*]], [[PARTWORD_CMPXCHG_FAILURE:%.*]] ]
; CHECK-NEXT:    [[TMP11:%.*]] = or i32 [[TMP10]], [[TMP5]]
; CHECK-NEXT:    [[TMP12:%.*]] = or i32 [[TMP10]], [[TMP7]]
; CHECK-NEXT:    [[TMP13:%.*]] = cmpxchg i32* [[ALIGNEDADDR]], i32 [[TMP12]], i32 [[TMP11]] monotonic monotonic, align 4
; CHECK-NEXT:    [[TMP14:%.*]] = extractvalue { i32, i1 } [[TMP13]], 0
; CHECK-NEXT:    [[TMP15:%.*]] = extractvalue { i32, i1 } [[TMP13]], 1
; CHECK-NEXT:    br i1 [[TMP15]], label [[PARTWORD_CMPXCHG_END:%.*]], label [[PARTWORD_CMPXCHG_FAILURE]]
; CHECK:       partword.cmpxchg.failure:
; CHECK-NEXT:    [[TMP16]] = and i32 [[TMP14]], [[INV_MASK]]
; CHECK-NEXT:    [[TMP17:%.*]] = icmp ne i32 [[TMP10]], [[TMP16]]
; CHECK-NEXT:    br i1 [[TMP17]], label [[PARTWORD_CMPXCHG_LOOP]], label [[PARTWORD_CMPXCHG_END]]
; CHECK:       partword.cmpxchg.end:
; CHECK-NEXT:    [[SHIFTED:%.*]] = lshr i32 [[TMP14]], [[SHIFTAMT]]
; CHECK-NEXT:    [[EXTRACTED:%.*]] = trunc i32 [[SHIFTED]] to i8
; CHECK-NEXT:    [[TMP18:%.*]] = insertvalue { i8, i1 } undef, i8 [[EXTRACTED]], 0
; CHECK-NEXT:    [[TMP19:%.*]] = insertvalue { i8, i1 } [[TMP18]], i1 [[TMP15]], 1
; CHECK-NEXT:    fence seq_cst
; CHECK-NEXT:    [[RET:%.*]] = extractvalue { i8, i1 } [[TMP19]], 0
; CHECK-NEXT:    ret i8 [[RET]]
;
entry:
  %ret_succ = cmpxchg i8* %arg, i8 %old, i8 %new seq_cst monotonic
  %ret = extractvalue { i8, i1 } %ret_succ, 0
  ret i8 %ret
}

define i16 @test_cmpxchg_i16(i16* %arg, i16 %old, i16 %new) {
; CHECK-LABEL: @test_cmpxchg_i16(
; CHECK-NEXT:  entry:
; CHECK-NEXT:    fence seq_cst
; CHECK-NEXT:    [[TMP0:%.*]] = ptrtoint i16* [[ARG:%.*]] to i64
; CHECK-NEXT:    [[TMP1:%.*]] = and i64 [[TMP0]], -4
; CHECK-NEXT:    [[ALIGNEDADDR:%.*]] = inttoptr i64 [[TMP1]] to i32*
; CHECK-NEXT:    [[PTRLSB:%.*]] = and i64 [[TMP0]], 3
; CHECK-NEXT:    [[TMP2:%.*]] = xor i64 [[PTRLSB]], 2
; CHECK-NEXT:    [[TMP3:%.*]] = shl i64 [[TMP2]], 3
; CHECK-NEXT:    [[SHIFTAMT:%.*]] = trunc i64 [[TMP3]] to i32
; CHECK-NEXT:    [[MASK:%.*]] = shl i32 65535, [[SHIFTAMT]]
; CHECK-NEXT:    [[INV_MASK:%.*]] = xor i32 [[MASK]], -1
; CHECK-NEXT:    [[TMP4:%.*]] = zext i16 [[NEW:%.*]] to i32
; CHECK-NEXT:    [[TMP5:%.*]] = shl i32 [[TMP4]], [[SHIFTAMT]]
; CHECK-NEXT:    [[TMP6:%.*]] = zext i16 [[OLD:%.*]] to i32
; CHECK-NEXT:    [[TMP7:%.*]] = shl i32 [[TMP6]], [[SHIFTAMT]]
; CHECK-NEXT:    [[TMP8:%.*]] = load i32, i32* [[ALIGNEDADDR]], align 4
; CHECK-NEXT:    [[TMP9:%.*]] = and i32 [[TMP8]], [[INV_MASK]]
; CHECK-NEXT:    br label [[PARTWORD_CMPXCHG_LOOP:%.*]]
; CHECK:       partword.cmpxchg.loop:
; CHECK-NEXT:    [[TMP10:%.*]] = phi i32 [ [[TMP9]], [[ENTRY:%.*]] ], [ [[TMP16:%.*]], [[PARTWORD_CMPXCHG_FAILURE:%.*]] ]
; CHECK-NEXT:    [[TMP11:%.*]] = or i32 [[TMP10]], [[TMP5]]
; CHECK-NEXT:    [[TMP12:%.*]] = or i32 [[TMP10]], [[TMP7]]
; CHECK-NEXT:    [[TMP13:%.*]] = cmpxchg i32* [[ALIGNEDADDR]], i32 [[TMP12]], i32 [[TMP11]] monotonic monotonic, align 4
; CHECK-NEXT:    [[TMP14:%.*]] = extractvalue { i32, i1 } [[TMP13]], 0
; CHECK-NEXT:    [[TMP15:%.*]] = extractvalue { i32, i1 } [[TMP13]], 1
; CHECK-NEXT:    br i1 [[TMP15]], label [[PARTWORD_CMPXCHG_END:%.*]], label [[PARTWORD_CMPXCHG_FAILURE]]
; CHECK:       partword.cmpxchg.failure:
; CHECK-NEXT:    [[TMP16]] = and i32 [[TMP14]], [[INV_MASK]]
; CHECK-NEXT:    [[TMP17:%.*]] = icmp ne i32 [[TMP10]], [[TMP16]]
; CHECK-NEXT:    br i1 [[TMP17]], label [[PARTWORD_CMPXCHG_LOOP]], label [[PARTWORD_CMPXCHG_END]]
; CHECK:       partword.cmpxchg.end:
; CHECK-NEXT:    [[SHIFTED:%.*]] = lshr i32 [[TMP14]], [[SHIFTAMT]]
; CHECK-NEXT:    [[EXTRACTED:%.*]] = trunc i32 [[SHIFTED]] to i16
; CHECK-NEXT:    [[TMP18:%.*]] = insertvalue { i16, i1 } undef, i16 [[EXTRACTED]], 0
; CHECK-NEXT:    [[TMP19:%.*]] = insertvalue { i16, i1 } [[TMP18]], i1 [[TMP15]], 1
; CHECK-NEXT:    fence seq_cst
; CHECK-NEXT:    [[RET:%.*]] = extractvalue { i16, i1 } [[TMP19]], 0
; CHECK-NEXT:    ret i16 [[RET]]
;
entry:
  %ret_succ = cmpxchg i16* %arg, i16 %old, i16 %new seq_cst monotonic
  %ret = extractvalue { i16, i1 } %ret_succ, 0
  ret i16 %ret
}

define i16 @test_add_i16(i16* %arg, i16 %val) {
; CHECK-LABEL: @test_add_i16(
; CHECK-NEXT:  entry:
; CHECK-NEXT:    fence seq_cst
; CHECK-NEXT:    [[TMP0:%.*]] = ptrtoint i16* [[ARG:%.*]] to i64
; CHECK-NEXT:    [[TMP1:%.*]] = and i64 [[TMP0]], -4
; CHECK-NEXT:    [[ALIGNEDADDR:%.*]] = inttoptr i64 [[TMP1]] to i32*
; CHECK-NEXT:    [[PTRLSB:%.*]] = and i64 [[TMP0]], 3
; CHECK-NEXT:    [[TMP2:%.*]] = xor i64 [[PTRLSB]], 2
; CHECK-NEXT:    [[TMP3:%.*]] = shl i64 [[TMP2]], 3
; CHECK-NEXT:    [[SHIFTAMT:%.*]] = trunc i64 [[TMP3]] to i32
; CHECK-NEXT:    [[MASK:%.*]] = shl i32 65535, [[SHIFTAMT]]
; CHECK-NEXT:    [[INV_MASK:%.*]] = xor i32 [[MASK]], -1
; CHECK-NEXT:    [[TMP4:%.*]] = zext i16 [[VAL:%.*]] to i32
; CHECK-NEXT:    [[VALOPERAND_SHIFTED:%.*]] = shl i32 [[TMP4]], [[SHIFTAMT]]
; CHECK-NEXT:    [[TMP5:%.*]] = load i32, i32* [[ALIGNEDADDR]], align 4
; CHECK-NEXT:    br label [[ATOMICRMW_START:%.*]]
; CHECK:       atomicrmw.start:
; CHECK-NEXT:    [[LOADED:%.*]] = phi i32 [ [[TMP5]], [[ENTRY:%.*]] ], [ [[NEWLOADED:%.*]], [[ATOMICRMW_START]] ]
; CHECK-NEXT:    [[NEW:%.*]] = add i32 [[LOADED]], [[VALOPERAND_SHIFTED]]
; CHECK-NEXT:    [[TMP6:%.*]] = and i32 [[NEW]], [[MASK]]
; CHECK-NEXT:    [[TMP7:%.*]] = and i32 [[LOADED]], [[INV_MASK]]
; CHECK-NEXT:    [[TMP8:%.*]] = or i32 [[TMP7]], [[TMP6]]
; CHECK-NEXT:    [[TMP9:%.*]] = cmpxchg i32* [[ALIGNEDADDR]], i32 [[LOADED]], i32 [[TMP8]] monotonic monotonic, align 4
; CHECK-NEXT:    [[SUCCESS:%.*]] = extractvalue { i32, i1 } [[TMP9]], 1
; CHECK-NEXT:    [[NEWLOADED]] = extractvalue { i32, i1 } [[TMP9]], 0
; CHECK-NEXT:    br i1 [[SUCCESS]], label [[ATOMICRMW_END:%.*]], label [[ATOMICRMW_START]]
; CHECK:       atomicrmw.end:
; CHECK-NEXT:    [[SHIFTED:%.*]] = lshr i32 [[NEWLOADED]], [[SHIFTAMT]]
; CHECK-NEXT:    [[EXTRACTED:%.*]] = trunc i32 [[SHIFTED]] to i16
; CHECK-NEXT:    fence seq_cst
; CHECK-NEXT:    ret i16 [[EXTRACTED]]
;
entry:
  %ret = atomicrmw add i16* %arg, i16 %val seq_cst
  ret i16 %ret
}

define i16 @test_xor_i16(i16* %arg, i16 %val) {
; CHECK-LABEL: @test_xor_i16(
; CHECK-NEXT:  entry:
; CHECK-NEXT:    fence seq_cst
; CHECK-NEXT:    [[TMP0:%.*]] = ptrtoint i16* [[ARG:%.*]] to i64
; CHECK-NEXT:    [[TMP1:%.*]] = and i64 [[TMP0]], -4
; CHECK-NEXT:    [[ALIGNEDADDR:%.*]] = inttoptr i64 [[TMP1]] to i32*
; CHECK-NEXT:    [[PTRLSB:%.*]] = and i64 [[TMP0]], 3
; CHECK-NEXT:    [[TMP2:%.*]] = xor i64 [[PTRLSB]], 2
; CHECK-NEXT:    [[TMP3:%.*]] = shl i64 [[TMP2]], 3
; CHECK-NEXT:    [[SHIFTAMT:%.*]] = trunc i64 [[TMP3]] to i32
; CHECK-NEXT:    [[MASK:%.*]] = shl i32 65535, [[SHIFTAMT]]
; CHECK-NEXT:    [[INV_MASK:%.*]] = xor i32 [[MASK]], -1
; CHECK-NEXT:    [[TMP4:%.*]] = zext i16 [[VAL:%.*]] to i32
; CHECK-NEXT:    [[VALOPERAND_SHIFTED:%.*]] = shl i32 [[TMP4]], [[SHIFTAMT]]
; CHECK-NEXT:    [[TMP5:%.*]] = load i32, i32* [[ALIGNEDADDR]], align 4
; CHECK-NEXT:    br label [[ATOMICRMW_START:%.*]]
; CHECK:       atomicrmw.start:
; CHECK-NEXT:    [[LOADED:%.*]] = phi i32 [ [[TMP5]], [[ENTRY:%.*]] ], [ [[NEWLOADED:%.*]], [[ATOMICRMW_START]] ]
; CHECK-NEXT:    [[NEW:%.*]] = xor i32 [[LOADED]], [[VALOPERAND_SHIFTED]]
; CHECK-NEXT:    [[TMP6:%.*]] = cmpxchg i32* [[ALIGNEDADDR]], i32 [[LOADED]], i32 [[NEW]] monotonic monotonic, align 4
; CHECK-NEXT:    [[SUCCESS:%.*]] = extractvalue { i32, i1 } [[TMP6]], 1
; CHECK-NEXT:    [[NEWLOADED]] = extractvalue { i32, i1 } [[TMP6]], 0
; CHECK-NEXT:    br i1 [[SUCCESS]], label [[ATOMICRMW_END:%.*]], label [[ATOMICRMW_START]]
; CHECK:       atomicrmw.end:
; CHECK-NEXT:    [[SHIFTED:%.*]] = lshr i32 [[NEWLOADED]], [[SHIFTAMT]]
; CHECK-NEXT:    [[EXTRACTED:%.*]] = trunc i32 [[SHIFTED]] to i16
; CHECK-NEXT:    fence seq_cst
; CHECK-NEXT:    ret i16 [[EXTRACTED]]
;
entry:
  %ret = atomicrmw xor i16* %arg, i16 %val seq_cst
  ret i16 %ret
}

define i16 @test_or_i16(i16* %arg, i16 %val) {
; CHECK-LABEL: @test_or_i16(
; CHECK-NEXT:  entry:
; CHECK-NEXT:    fence seq_cst
; CHECK-NEXT:    [[TMP0:%.*]] = ptrtoint i16* [[ARG:%.*]] to i64
; CHECK-NEXT:    [[TMP1:%.*]] = and i64 [[TMP0]], -4
; CHECK-NEXT:    [[ALIGNEDADDR:%.*]] = inttoptr i64 [[TMP1]] to i32*
; CHECK-NEXT:    [[PTRLSB:%.*]] = and i64 [[TMP0]], 3
; CHECK-NEXT:    [[TMP2:%.*]] = xor i64 [[PTRLSB]], 2
; CHECK-NEXT:    [[TMP3:%.*]] = shl i64 [[TMP2]], 3
; CHECK-NEXT:    [[SHIFTAMT:%.*]] = trunc i64 [[TMP3]] to i32
; CHECK-NEXT:    [[MASK:%.*]] = shl i32 65535, [[SHIFTAMT]]
; CHECK-NEXT:    [[INV_MASK:%.*]] = xor i32 [[MASK]], -1
; CHECK-NEXT:    [[TMP4:%.*]] = zext i16 [[VAL:%.*]] to i32
; CHECK-NEXT:    [[VALOPERAND_SHIFTED:%.*]] = shl i32 [[TMP4]], [[SHIFTAMT]]
; CHECK-NEXT:    [[TMP5:%.*]] = load i32, i32* [[ALIGNEDADDR]], align 4
; CHECK-NEXT:    br label [[ATOMICRMW_START:%.*]]
; CHECK:       atomicrmw.start:
; CHECK-NEXT:    [[LOADED:%.*]] = phi i32 [ [[TMP5]], [[ENTRY:%.*]] ], [ [[NEWLOADED:%.*]], [[ATOMICRMW_START]] ]
; CHECK-NEXT:    [[NEW:%.*]] = or i32 [[LOADED]], [[VALOPERAND_SHIFTED]]
; CHECK-NEXT:    [[TMP6:%.*]] = cmpxchg i32* [[ALIGNEDADDR]], i32 [[LOADED]], i32 [[NEW]] monotonic monotonic, align 4
; CHECK-NEXT:    [[SUCCESS:%.*]] = extractvalue { i32, i1 } [[TMP6]], 1
; CHECK-NEXT:    [[NEWLOADED]] = extractvalue { i32, i1 } [[TMP6]], 0
; CHECK-NEXT:    br i1 [[SUCCESS]], label [[ATOMICRMW_END:%.*]], label [[ATOMICRMW_START]]
; CHECK:       atomicrmw.end:
; CHECK-NEXT:    [[SHIFTED:%.*]] = lshr i32 [[NEWLOADED]], [[SHIFTAMT]]
; CHECK-NEXT:    [[EXTRACTED:%.*]] = trunc i32 [[SHIFTED]] to i16
; CHECK-NEXT:    fence seq_cst
; CHECK-NEXT:    ret i16 [[EXTRACTED]]
;
entry:
  %ret = atomicrmw or i16* %arg, i16 %val seq_cst
  ret i16 %ret
}

define i16 @test_and_i16(i16* %arg, i16 %val) {
; CHECK-LABEL: @test_and_i16(
; CHECK-NEXT:  entry:
; CHECK-NEXT:    fence seq_cst
; CHECK-NEXT:    [[TMP0:%.*]] = ptrtoint i16* [[ARG:%.*]] to i64
; CHECK-NEXT:    [[TMP1:%.*]] = and i64 [[TMP0]], -4
; CHECK-NEXT:    [[ALIGNEDADDR:%.*]] = inttoptr i64 [[TMP1]] to i32*
; CHECK-NEXT:    [[PTRLSB:%.*]] = and i64 [[TMP0]], 3
; CHECK-NEXT:    [[TMP2:%.*]] = xor i64 [[PTRLSB]], 2
; CHECK-NEXT:    [[TMP3:%.*]] = shl i64 [[TMP2]], 3
; CHECK-NEXT:    [[SHIFTAMT:%.*]] = trunc i64 [[TMP3]] to i32
; CHECK-NEXT:    [[MASK:%.*]] = shl i32 65535, [[SHIFTAMT]]
; CHECK-NEXT:    [[INV_MASK:%.*]] = xor i32 [[MASK]], -1
; CHECK-NEXT:    [[TMP4:%.*]] = zext i16 [[VAL:%.*]] to i32
; CHECK-NEXT:    [[VALOPERAND_SHIFTED:%.*]] = shl i32 [[TMP4]], [[SHIFTAMT]]
; CHECK-NEXT:    [[ANDOPERAND:%.*]] = or i32 [[INV_MASK]], [[VALOPERAND_SHIFTED]]
; CHECK-NEXT:    [[TMP5:%.*]] = load i32, i32* [[ALIGNEDADDR]], align 4
; CHECK-NEXT:    br label [[ATOMICRMW_START:%.*]]
; CHECK:       atomicrmw.start:
; CHECK-NEXT:    [[LOADED:%.*]] = phi i32 [ [[TMP5]], [[ENTRY:%.*]] ], [ [[NEWLOADED:%.*]], [[ATOMICRMW_START]] ]
; CHECK-NEXT:    [[NEW:%.*]] = and i32 [[LOADED]], [[ANDOPERAND]]
; CHECK-NEXT:    [[TMP6:%.*]] = cmpxchg i32* [[ALIGNEDADDR]], i32 [[LOADED]], i32 [[NEW]] monotonic monotonic, align 4
; CHECK-NEXT:    [[SUCCESS:%.*]] = extractvalue { i32, i1 } [[TMP6]], 1
; CHECK-NEXT:    [[NEWLOADED]] = extractvalue { i32, i1 } [[TMP6]], 0
; CHECK-NEXT:    br i1 [[SUCCESS]], label [[ATOMICRMW_END:%.*]], label [[ATOMICRMW_START]]
; CHECK:       atomicrmw.end:
; CHECK-NEXT:    [[SHIFTED:%.*]] = lshr i32 [[NEWLOADED]], [[SHIFTAMT]]
; CHECK-NEXT:    [[EXTRACTED:%.*]] = trunc i32 [[SHIFTED]] to i16
; CHECK-NEXT:    fence seq_cst
; CHECK-NEXT:    ret i16 [[EXTRACTED]]
;
entry:
  %ret = atomicrmw and i16* %arg, i16 %val seq_cst
  ret i16 %ret
}

define i16 @test_min_i16(i16* %arg, i16 %val) {
; CHECK-LABEL: @test_min_i16(
; CHECK-NEXT:  entry:
; CHECK-NEXT:    fence seq_cst
; CHECK-NEXT:    [[TMP0:%.*]] = ptrtoint i16* [[ARG:%.*]] to i64
; CHECK-NEXT:    [[TMP1:%.*]] = and i64 [[TMP0]], -4
; CHECK-NEXT:    [[ALIGNEDADDR:%.*]] = inttoptr i64 [[TMP1]] to i32*
; CHECK-NEXT:    [[PTRLSB:%.*]] = and i64 [[TMP0]], 3
; CHECK-NEXT:    [[TMP2:%.*]] = xor i64 [[PTRLSB]], 2
; CHECK-NEXT:    [[TMP3:%.*]] = shl i64 [[TMP2]], 3
; CHECK-NEXT:    [[SHIFTAMT:%.*]] = trunc i64 [[TMP3]] to i32
; CHECK-NEXT:    [[MASK:%.*]] = shl i32 65535, [[SHIFTAMT]]
; CHECK-NEXT:    [[INV_MASK:%.*]] = xor i32 [[MASK]], -1
; CHECK-NEXT:    [[TMP4:%.*]] = zext i16 [[VAL:%.*]] to i32
; CHECK-NEXT:    [[VALOPERAND_SHIFTED:%.*]] = shl i32 [[TMP4]], [[SHIFTAMT]]
; CHECK-NEXT:    [[TMP5:%.*]] = load i32, i32* [[ALIGNEDADDR]], align 4
; CHECK-NEXT:    br label [[ATOMICRMW_START:%.*]]
; CHECK:       atomicrmw.start:
; CHECK-NEXT:    [[LOADED:%.*]] = phi i32 [ [[TMP5]], [[ENTRY:%.*]] ], [ [[NEWLOADED:%.*]], [[ATOMICRMW_START]] ]
; CHECK-NEXT:    [[SHIFTED:%.*]] = lshr i32 [[LOADED]], [[SHIFTAMT]]
; CHECK-NEXT:    [[EXTRACTED:%.*]] = trunc i32 [[SHIFTED]] to i16
; CHECK-NEXT:    [[TMP6:%.*]] = icmp sle i16 [[EXTRACTED]], [[VAL]]
; CHECK-NEXT:    [[NEW:%.*]] = select i1 [[TMP6]], i16 [[EXTRACTED]], i16 [[VAL]]
; CHECK-NEXT:    [[EXTENDED:%.*]] = zext i16 [[NEW]] to i32
; CHECK-NEXT:    [[SHIFTED1:%.*]] = shl nuw i32 [[EXTENDED]], [[SHIFTAMT]]
; CHECK-NEXT:    [[UNMASKED:%.*]] = and i32 [[LOADED]], [[INV_MASK]]
; CHECK-NEXT:    [[INSERTED:%.*]] = or i32 [[UNMASKED]], [[SHIFTED1]]
; CHECK-NEXT:    [[TMP7:%.*]] = cmpxchg i32* [[ALIGNEDADDR]], i32 [[LOADED]], i32 [[INSERTED]] monotonic monotonic, align 4
; CHECK-NEXT:    [[SUCCESS:%.*]] = extractvalue { i32, i1 } [[TMP7]], 1
; CHECK-NEXT:    [[NEWLOADED]] = extractvalue { i32, i1 } [[TMP7]], 0
; CHECK-NEXT:    br i1 [[SUCCESS]], label [[ATOMICRMW_END:%.*]], label [[ATOMICRMW_START]]
; CHECK:       atomicrmw.end:
; CHECK-NEXT:    [[SHIFTED2:%.*]] = lshr i32 [[NEWLOADED]], [[SHIFTAMT]]
; CHECK-NEXT:    [[EXTRACTED3:%.*]] = trunc i32 [[SHIFTED2]] to i16
; CHECK-NEXT:    fence seq_cst
; CHECK-NEXT:    ret i16 [[EXTRACTED3]]
;
entry:
  %ret = atomicrmw min i16* %arg, i16 %val seq_cst
  ret i16 %ret
}
