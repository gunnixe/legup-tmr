; ModuleID = 'adpcm.prelto.6.bc'
target datalayout = "e-m:e-p:32:32-f64:32:64-f80:32-n8:16:32-S128"
target triple = "i386-unknown-linux-gnu"

@qq4_code4_table = internal unnamed_addr constant [16 x i32] [i32 0, i32 -20456, i32 -12896, i32 -8968, i32 -6288, i32 -4240, i32 -2584, i32 -1200, i32 20456, i32 12896, i32 8968, i32 6288, i32 4240, i32 2584, i32 1200, i32 0], align 4
@qq6_code6_table = internal unnamed_addr constant [64 x i32] [i32 -136, i32 -136, i32 -136, i32 -136, i32 -24808, i32 -21904, i32 -19008, i32 -16704, i32 -14984, i32 -13512, i32 -12280, i32 -11192, i32 -10232, i32 -9360, i32 -8576, i32 -7856, i32 -7192, i32 -6576, i32 -6000, i32 -5456, i32 -4944, i32 -4464, i32 -4008, i32 -3576, i32 -3168, i32 -2776, i32 -2400, i32 -2032, i32 -1688, i32 -1360, i32 -1040, i32 -728, i32 24808, i32 21904, i32 19008, i32 16704, i32 14984, i32 13512, i32 12280, i32 11192, i32 10232, i32 9360, i32 8576, i32 7856, i32 7192, i32 6576, i32 6000, i32 5456, i32 4944, i32 4464, i32 4008, i32 3576, i32 3168, i32 2776, i32 2400, i32 2032, i32 1688, i32 1360, i32 1040, i32 728, i32 432, i32 136, i32 -432, i32 -136], align 4
@wl_code_table = internal unnamed_addr constant [16 x i32] [i32 -60, i32 3042, i32 1198, i32 538, i32 334, i32 172, i32 58, i32 -30, i32 3042, i32 1198, i32 538, i32 334, i32 172, i32 58, i32 -30, i32 -60], align 4
@ilb_table = internal unnamed_addr constant [32 x i32] [i32 2048, i32 2093, i32 2139, i32 2186, i32 2233, i32 2282, i32 2332, i32 2383, i32 2435, i32 2489, i32 2543, i32 2599, i32 2656, i32 2714, i32 2774, i32 2834, i32 2896, i32 2960, i32 3025, i32 3091, i32 3158, i32 3228, i32 3298, i32 3371, i32 3444, i32 3520, i32 3597, i32 3676, i32 3756, i32 3838, i32 3922, i32 4008], align 4
@decis_levl = internal unnamed_addr constant [30 x i32] [i32 280, i32 576, i32 880, i32 1200, i32 1520, i32 1864, i32 2208, i32 2584, i32 2960, i32 3376, i32 3784, i32 4240, i32 4696, i32 5200, i32 5712, i32 6288, i32 6864, i32 7520, i32 8184, i32 8968, i32 9752, i32 10712, i32 11664, i32 12896, i32 14120, i32 15840, i32 17560, i32 20456, i32 23352, i32 32767], align 4
@quant26bt_pos = internal unnamed_addr constant [31 x i32] [i32 61, i32 60, i32 59, i32 58, i32 57, i32 56, i32 55, i32 54, i32 53, i32 52, i32 51, i32 50, i32 49, i32 48, i32 47, i32 46, i32 45, i32 44, i32 43, i32 42, i32 41, i32 40, i32 39, i32 38, i32 37, i32 36, i32 35, i32 34, i32 33, i32 32, i32 32], align 4
@quant26bt_neg = internal unnamed_addr constant [31 x i32] [i32 63, i32 62, i32 31, i32 30, i32 29, i32 28, i32 27, i32 26, i32 25, i32 24, i32 23, i32 22, i32 21, i32 20, i32 19, i32 18, i32 17, i32 16, i32 15, i32 14, i32 13, i32 12, i32 11, i32 10, i32 9, i32 8, i32 7, i32 6, i32 5, i32 4, i32 4], align 4
@qq2_code2_table = internal unnamed_addr constant [4 x i32] [i32 -7408, i32 -1616, i32 7408, i32 1616], align 4
@wh_code_table = internal unnamed_addr constant [4 x i32] [i32 798, i32 -214, i32 798, i32 -214], align 4
@tqmf = internal unnamed_addr global [24 x i32] zeroinitializer, align 4
@delay_bpl = internal global [6 x i32] zeroinitializer, align 4
@delay_dltx = internal global [6 x i32] zeroinitializer, align 4
@delay_bph = internal global [6 x i32] zeroinitializer, align 4
@delay_dhx = internal global [6 x i32] zeroinitializer, align 4
@dec_del_bpl = internal global [6 x i32] zeroinitializer, align 4
@dec_del_dltx = internal global [6 x i32] zeroinitializer, align 4
@dec_del_bph = internal global [6 x i32] zeroinitializer, align 4
@dec_del_dhx = internal global [6 x i32] zeroinitializer, align 4
@test_data = internal unnamed_addr constant [100 x i32] [i32 68, i32 68, i32 68, i32 68, i32 68, i32 68, i32 68, i32 68, i32 68, i32 68, i32 68, i32 68, i32 68, i32 68, i32 68, i32 68, i32 68, i32 67, i32 67, i32 67, i32 67, i32 67, i32 67, i32 67, i32 66, i32 66, i32 66, i32 66, i32 66, i32 66, i32 65, i32 65, i32 65, i32 65, i32 65, i32 64, i32 64, i32 64, i32 64, i32 64, i32 64, i32 64, i32 64, i32 63, i32 63, i32 63, i32 63, i32 63, i32 62, i32 62, i32 62, i32 62, i32 62, i32 62, i32 61, i32 61, i32 61, i32 61, i32 61, i32 61, i32 60, i32 60, i32 60, i32 60, i32 60, i32 60, i32 60, i32 60, i32 60, i32 59, i32 59, i32 59, i32 59, i32 59, i32 59, i32 59, i32 59, i32 59, i32 59, i32 59, i32 59, i32 59, i32 59, i32 59, i32 59, i32 59, i32 59, i32 59, i32 59, i32 59, i32 59, i32 59, i32 60, i32 60, i32 60, i32 60, i32 60, i32 60, i32 60, i32 60], align 4
@test_compressed = internal unnamed_addr constant [100 x i32] [i32 253, i32 222, i32 119, i32 186, i32 244, i32 146, i32 32, i32 160, i32 236, i32 237, i32 238, i32 240, i32 241, i32 241, i32 242, i32 243, i32 244, i32 243, i32 244, i32 245, i32 244, i32 244, i32 245, i32 245, i32 245, i32 246, i32 246, i32 247, i32 247, i32 247, i32 247, i32 248, i32 246, i32 247, i32 249, i32 247, i32 248, i32 247, i32 248, i32 247, i32 248, i32 247, i32 248, i32 247, i32 248, i32 248, i32 246, i32 248, i32 247, i32 248, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0], align 4
@test_result = internal unnamed_addr constant [100 x i32] [i32 0, i32 -1, i32 -1, i32 0, i32 0, i32 -1, i32 0, i32 0, i32 -1, i32 -1, i32 0, i32 0, i32 1, i32 1, i32 0, i32 -2, i32 -1, i32 -2, i32 0, i32 -3, i32 1, i32 0, i32 0, i32 -4, i32 1, i32 1, i32 2, i32 11, i32 20, i32 18, i32 20, i32 22, i32 28, i32 27, i32 31, i32 31, i32 34, i32 31, i32 34, i32 34, i32 38, i32 37, i32 42, i32 42, i32 44, i32 41, i32 43, i32 42, i32 47, i32 45, i32 47, i32 44, i32 45, i32 43, i32 46, i32 45, i32 48, i32 46, i32 49, i32 48, i32 51, i32 49, i32 52, i32 52, i32 55, i32 53, i32 56, i32 55, i32 58, i32 57, i32 59, i32 57, i32 60, i32 60, i32 60, i32 54, i32 54, i32 53, i32 60, i32 62, i32 62, i32 54, i32 55, i32 56, i32 59, i32 53, i32 54, i32 56, i32 59, i32 53, i32 56, i32 58, i32 59, i32 53, i32 56, i32 58, i32 60, i32 54, i32 55, i32 57], align 4
@compressed = internal unnamed_addr global [100 x i32] zeroinitializer, align 4
@result = internal unnamed_addr global [100 x i32] zeroinitializer, align 4
@.str = private unnamed_addr constant [12 x i8] c"Result: %d\0A\00", align 1
@.str1 = private unnamed_addr constant [14 x i8] c"RESULT: PASS\0A\00", align 1
@.str2 = private unnamed_addr constant [14 x i8] c"RESULT: FAIL\0A\00", align 1

; Function Attrs: nounwind
define internal fastcc void @upzero(i32 %dlt, i32* nocapture %dlti, i32* nocapture %bli) #0 {
  %1 = icmp eq i32 %dlt, 0
  br i1 %1, label %.preheader, label %.preheader1

.preheader1:                                      ; preds = %0
  %2 = load i32* %dlti, align 4, !tbaa !1
  %3 = mul nsw i32 %2, %dlt
  %4 = ashr i32 %3, 31
  %5 = and i32 %4, -256
  %6 = or i32 %5, 128
  %7 = load i32* %bli, align 4, !tbaa !1
  %8 = mul nsw i32 %7, 255
  %9 = ashr i32 %8, 8
  %10 = add nsw i32 %6, %9
  store i32 %10, i32* %bli, align 4, !tbaa !1
  %11 = getelementptr inbounds i32* %dlti, i32 1
  %12 = load i32* %11, align 4, !tbaa !1
  %13 = mul nsw i32 %12, %dlt
  %14 = ashr i32 %13, 31
  %15 = and i32 %14, -256
  %16 = or i32 %15, 128
  %17 = getelementptr inbounds i32* %bli, i32 1
  %18 = load i32* %17, align 4, !tbaa !1
  %19 = mul nsw i32 %18, 255
  %20 = ashr i32 %19, 8
  %21 = add nsw i32 %16, %20
  store i32 %21, i32* %17, align 4, !tbaa !1
  %22 = getelementptr inbounds i32* %dlti, i32 2
  %23 = load i32* %22, align 4, !tbaa !1
  %24 = mul nsw i32 %23, %dlt
  %25 = ashr i32 %24, 31
  %26 = and i32 %25, -256
  %27 = or i32 %26, 128
  %28 = getelementptr inbounds i32* %bli, i32 2
  %29 = load i32* %28, align 4, !tbaa !1
  %30 = mul nsw i32 %29, 255
  %31 = ashr i32 %30, 8
  %32 = add nsw i32 %27, %31
  store i32 %32, i32* %28, align 4, !tbaa !1
  %33 = getelementptr inbounds i32* %dlti, i32 3
  %34 = load i32* %33, align 4, !tbaa !1
  %35 = mul nsw i32 %34, %dlt
  %36 = ashr i32 %35, 31
  %37 = and i32 %36, -256
  %38 = or i32 %37, 128
  %39 = getelementptr inbounds i32* %bli, i32 3
  %40 = load i32* %39, align 4, !tbaa !1
  %41 = mul nsw i32 %40, 255
  %42 = ashr i32 %41, 8
  %43 = add nsw i32 %38, %42
  store i32 %43, i32* %39, align 4, !tbaa !1
  %44 = getelementptr inbounds i32* %dlti, i32 4
  %45 = load i32* %44, align 4, !tbaa !1
  %46 = mul nsw i32 %45, %dlt
  %47 = ashr i32 %46, 31
  %48 = and i32 %47, -256
  %49 = or i32 %48, 128
  %50 = getelementptr inbounds i32* %bli, i32 4
  %51 = load i32* %50, align 4, !tbaa !1
  %52 = mul nsw i32 %51, 255
  %53 = ashr i32 %52, 8
  %54 = add nsw i32 %49, %53
  store i32 %54, i32* %50, align 4, !tbaa !1
  %55 = getelementptr inbounds i32* %dlti, i32 5
  %56 = load i32* %55, align 4, !tbaa !1
  %57 = mul nsw i32 %56, %dlt
  %58 = ashr i32 %57, 31
  %59 = and i32 %58, -256
  %60 = or i32 %59, 128
  %61 = getelementptr inbounds i32* %bli, i32 5
  %62 = load i32* %61, align 4, !tbaa !1
  %63 = mul nsw i32 %62, 255
  %64 = ashr i32 %63, 8
  %65 = add nsw i32 %60, %64
  store i32 %65, i32* %61, align 4, !tbaa !1
  br label %89

.preheader:                                       ; preds = %0
  %66 = load i32* %bli, align 4, !tbaa !1
  %67 = mul nsw i32 %66, 255
  %68 = ashr i32 %67, 8
  store i32 %68, i32* %bli, align 4, !tbaa !1
  %69 = getelementptr inbounds i32* %bli, i32 1
  %70 = load i32* %69, align 4, !tbaa !1
  %71 = mul nsw i32 %70, 255
  %72 = ashr i32 %71, 8
  store i32 %72, i32* %69, align 4, !tbaa !1
  %73 = getelementptr inbounds i32* %bli, i32 2
  %74 = load i32* %73, align 4, !tbaa !1
  %75 = mul nsw i32 %74, 255
  %76 = ashr i32 %75, 8
  store i32 %76, i32* %73, align 4, !tbaa !1
  %77 = getelementptr inbounds i32* %bli, i32 3
  %78 = load i32* %77, align 4, !tbaa !1
  %79 = mul nsw i32 %78, 255
  %80 = ashr i32 %79, 8
  store i32 %80, i32* %77, align 4, !tbaa !1
  %81 = getelementptr inbounds i32* %bli, i32 4
  %82 = load i32* %81, align 4, !tbaa !1
  %83 = mul nsw i32 %82, 255
  %84 = ashr i32 %83, 8
  store i32 %84, i32* %81, align 4, !tbaa !1
  %85 = getelementptr inbounds i32* %bli, i32 5
  %86 = load i32* %85, align 4, !tbaa !1
  %87 = mul nsw i32 %86, 255
  %88 = ashr i32 %87, 8
  store i32 %88, i32* %85, align 4, !tbaa !1
  %.pre = getelementptr inbounds i32* %dlti, i32 4
  %.pre7 = getelementptr inbounds i32* %dlti, i32 5
  %.pre9 = getelementptr inbounds i32* %dlti, i32 3
  %.pre11 = getelementptr inbounds i32* %dlti, i32 2
  %.pre13 = getelementptr inbounds i32* %dlti, i32 1
  br label %89

; <label>:89                                      ; preds = %.preheader, %.preheader1
  %.pre-phi14 = phi i32* [ %11, %.preheader1 ], [ %.pre13, %.preheader ]
  %.pre-phi12 = phi i32* [ %22, %.preheader1 ], [ %.pre11, %.preheader ]
  %.pre-phi10 = phi i32* [ %33, %.preheader1 ], [ %.pre9, %.preheader ]
  %.pre-phi8 = phi i32* [ %55, %.preheader1 ], [ %.pre7, %.preheader ]
  %.pre-phi = phi i32* [ %44, %.preheader1 ], [ %.pre, %.preheader ]
  %90 = load i32* %.pre-phi, align 4, !tbaa !1
  store i32 %90, i32* %.pre-phi8, align 4, !tbaa !1
  %91 = load i32* %.pre-phi10, align 4, !tbaa !1
  store i32 %91, i32* %.pre-phi, align 4, !tbaa !1
  %92 = load i32* %.pre-phi12, align 4, !tbaa !1
  store i32 %92, i32* %.pre-phi10, align 4, !tbaa !1
  %93 = load i32* %dlti, align 4, !tbaa !1
  store i32 %93, i32* %.pre-phi14, align 4, !tbaa !1
  store i32 %dlt, i32* %dlti, align 4, !tbaa !1
  ret void
}

; Function Attrs: nounwind
define i32 @main() #0 {
  store i32 0, i32* getelementptr inbounds ([6 x i32]* @delay_dltx, i32 0, i32 0), align 4, !tbaa !1
  store i32 0, i32* getelementptr inbounds ([6 x i32]* @delay_dhx, i32 0, i32 0), align 4, !tbaa !1
  store i32 0, i32* getelementptr inbounds ([6 x i32]* @dec_del_dltx, i32 0, i32 0), align 4, !tbaa !1
  store i32 0, i32* getelementptr inbounds ([6 x i32]* @dec_del_dhx, i32 0, i32 0), align 4, !tbaa !1
  store i32 0, i32* getelementptr inbounds ([6 x i32]* @delay_dltx, i32 0, i32 1), align 4, !tbaa !1
  store i32 0, i32* getelementptr inbounds ([6 x i32]* @delay_dhx, i32 0, i32 1), align 4, !tbaa !1
  store i32 0, i32* getelementptr inbounds ([6 x i32]* @dec_del_dltx, i32 0, i32 1), align 4, !tbaa !1
  store i32 0, i32* getelementptr inbounds ([6 x i32]* @dec_del_dhx, i32 0, i32 1), align 4, !tbaa !1
  store i32 0, i32* getelementptr inbounds ([6 x i32]* @delay_dltx, i32 0, i32 2), align 4, !tbaa !1
  store i32 0, i32* getelementptr inbounds ([6 x i32]* @delay_dhx, i32 0, i32 2), align 4, !tbaa !1
  store i32 0, i32* getelementptr inbounds ([6 x i32]* @dec_del_dltx, i32 0, i32 2), align 4, !tbaa !1
  store i32 0, i32* getelementptr inbounds ([6 x i32]* @dec_del_dhx, i32 0, i32 2), align 4, !tbaa !1
  store i32 0, i32* getelementptr inbounds ([6 x i32]* @delay_dltx, i32 0, i32 3), align 4, !tbaa !1
  store i32 0, i32* getelementptr inbounds ([6 x i32]* @delay_dhx, i32 0, i32 3), align 4, !tbaa !1
  store i32 0, i32* getelementptr inbounds ([6 x i32]* @dec_del_dltx, i32 0, i32 3), align 4, !tbaa !1
  store i32 0, i32* getelementptr inbounds ([6 x i32]* @dec_del_dhx, i32 0, i32 3), align 4, !tbaa !1
  store i32 0, i32* getelementptr inbounds ([6 x i32]* @delay_dltx, i32 0, i32 4), align 4, !tbaa !1
  store i32 0, i32* getelementptr inbounds ([6 x i32]* @delay_dhx, i32 0, i32 4), align 4, !tbaa !1
  store i32 0, i32* getelementptr inbounds ([6 x i32]* @dec_del_dltx, i32 0, i32 4), align 4, !tbaa !1
  store i32 0, i32* getelementptr inbounds ([6 x i32]* @dec_del_dhx, i32 0, i32 4), align 4, !tbaa !1
  store i32 0, i32* getelementptr inbounds ([6 x i32]* @delay_dltx, i32 0, i32 5), align 4, !tbaa !1
  store i32 0, i32* getelementptr inbounds ([6 x i32]* @delay_dhx, i32 0, i32 5), align 4, !tbaa !1
  store i32 0, i32* getelementptr inbounds ([6 x i32]* @dec_del_dltx, i32 0, i32 5), align 4, !tbaa !1
  store i32 0, i32* getelementptr inbounds ([6 x i32]* @dec_del_dhx, i32 0, i32 5), align 4, !tbaa !1
  store i32 0, i32* getelementptr inbounds ([6 x i32]* @delay_bpl, i32 0, i32 0), align 4, !tbaa !1
  store i32 0, i32* getelementptr inbounds ([6 x i32]* @delay_bph, i32 0, i32 0), align 4, !tbaa !1
  store i32 0, i32* getelementptr inbounds ([6 x i32]* @dec_del_bpl, i32 0, i32 0), align 4, !tbaa !1
  store i32 0, i32* getelementptr inbounds ([6 x i32]* @dec_del_bph, i32 0, i32 0), align 4, !tbaa !1
  store i32 0, i32* getelementptr inbounds ([6 x i32]* @delay_bpl, i32 0, i32 1), align 4, !tbaa !1
  store i32 0, i32* getelementptr inbounds ([6 x i32]* @delay_bph, i32 0, i32 1), align 4, !tbaa !1
  store i32 0, i32* getelementptr inbounds ([6 x i32]* @dec_del_bpl, i32 0, i32 1), align 4, !tbaa !1
  store i32 0, i32* getelementptr inbounds ([6 x i32]* @dec_del_bph, i32 0, i32 1), align 4, !tbaa !1
  store i32 0, i32* getelementptr inbounds ([6 x i32]* @delay_bpl, i32 0, i32 2), align 4, !tbaa !1
  store i32 0, i32* getelementptr inbounds ([6 x i32]* @delay_bph, i32 0, i32 2), align 4, !tbaa !1
  store i32 0, i32* getelementptr inbounds ([6 x i32]* @dec_del_bpl, i32 0, i32 2), align 4, !tbaa !1
  store i32 0, i32* getelementptr inbounds ([6 x i32]* @dec_del_bph, i32 0, i32 2), align 4, !tbaa !1
  store i32 0, i32* getelementptr inbounds ([6 x i32]* @delay_bpl, i32 0, i32 3), align 4, !tbaa !1
  store i32 0, i32* getelementptr inbounds ([6 x i32]* @delay_bph, i32 0, i32 3), align 4, !tbaa !1
  store i32 0, i32* getelementptr inbounds ([6 x i32]* @dec_del_bpl, i32 0, i32 3), align 4, !tbaa !1
  store i32 0, i32* getelementptr inbounds ([6 x i32]* @dec_del_bph, i32 0, i32 3), align 4, !tbaa !1
  store i32 0, i32* getelementptr inbounds ([6 x i32]* @delay_bpl, i32 0, i32 4), align 4, !tbaa !1
  store i32 0, i32* getelementptr inbounds ([6 x i32]* @delay_bph, i32 0, i32 4), align 4, !tbaa !1
  store i32 0, i32* getelementptr inbounds ([6 x i32]* @dec_del_bpl, i32 0, i32 4), align 4, !tbaa !1
  store i32 0, i32* getelementptr inbounds ([6 x i32]* @dec_del_bph, i32 0, i32 4), align 4, !tbaa !1
  store i32 0, i32* getelementptr inbounds ([6 x i32]* @delay_bpl, i32 0, i32 5), align 4, !tbaa !1
  store i32 0, i32* getelementptr inbounds ([6 x i32]* @delay_bph, i32 0, i32 5), align 4, !tbaa !1
  store i32 0, i32* getelementptr inbounds ([6 x i32]* @dec_del_bpl, i32 0, i32 5), align 4, !tbaa !1
  store i32 0, i32* getelementptr inbounds ([6 x i32]* @dec_del_bph, i32 0, i32 5), align 4, !tbaa !1
  %1 = bitcast i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 0) to i8*
  call void @legup_memset_4_i64(i8* %1, i8 0, i64 96)
  br label %2

; <label>:2                                       ; preds = %encode.exit._crit_edge, %0
  %3 = phi i32 [ 0, %0 ], [ %.pre59, %encode.exit._crit_edge ]
  %4 = phi i32 [ 0, %0 ], [ %.pre58, %encode.exit._crit_edge ]
  %5 = phi i32 [ 0, %0 ], [ %.pre57, %encode.exit._crit_edge ]
  %6 = phi i32 [ 0, %0 ], [ %.pre56, %encode.exit._crit_edge ]
  %7 = phi i32 [ 0, %0 ], [ %.pre55, %encode.exit._crit_edge ]
  %8 = phi i32 [ 0, %0 ], [ %.pre54, %encode.exit._crit_edge ]
  %9 = phi i32 [ 0, %0 ], [ %.pre53, %encode.exit._crit_edge ]
  %10 = phi i32 [ 0, %0 ], [ %.pre52, %encode.exit._crit_edge ]
  %11 = phi i32 [ 0, %0 ], [ %.pre51, %encode.exit._crit_edge ]
  %12 = phi i32 [ 0, %0 ], [ %.pre50, %encode.exit._crit_edge ]
  %13 = phi i32 [ 0, %0 ], [ %.pre49, %encode.exit._crit_edge ]
  %14 = phi i32 [ 0, %0 ], [ %.pre48, %encode.exit._crit_edge ]
  %15 = phi i32 [ 0, %0 ], [ %.pre47, %encode.exit._crit_edge ]
  %16 = phi i32 [ 0, %0 ], [ %.pre46, %encode.exit._crit_edge ]
  %17 = phi i32 [ 0, %0 ], [ %.pre45, %encode.exit._crit_edge ]
  %18 = phi i32 [ 0, %0 ], [ %.pre44, %encode.exit._crit_edge ]
  %19 = phi i32 [ 0, %0 ], [ %.pre43, %encode.exit._crit_edge ]
  %20 = phi i32 [ 0, %0 ], [ %.pre42, %encode.exit._crit_edge ]
  %21 = phi i32 [ 0, %0 ], [ %.pre41, %encode.exit._crit_edge ]
  %22 = phi i32 [ 0, %0 ], [ %.pre40, %encode.exit._crit_edge ]
  %23 = phi i32 [ 0, %0 ], [ %.pre39, %encode.exit._crit_edge ]
  %24 = phi i32 [ 0, %0 ], [ %.pre38, %encode.exit._crit_edge ]
  %25 = phi i32 [ 0, %0 ], [ %.pre37, %encode.exit._crit_edge ]
  %26 = phi i32 [ 0, %0 ], [ %.pre, %encode.exit._crit_edge ]
  %ph2.0 = phi i32 [ 0, %0 ], [ %ph1.0, %encode.exit._crit_edge ]
  %ph1.0 = phi i32 [ 0, %0 ], [ %224, %encode.exit._crit_edge ]
  %nbh.0 = phi i32 [ 0, %0 ], [ %...i2.i15, %encode.exit._crit_edge ]
  %deth.0 = phi i32 [ 8, %0 ], [ %223, %encode.exit._crit_edge ]
  %ah2.0 = phi i32 [ 0, %0 ], [ %...i.i19, %encode.exit._crit_edge ]
  %rh2.0 = phi i32 [ 0, %0 ], [ %rh1.0, %encode.exit._crit_edge ]
  %ah1.0 = phi i32 [ 0, %0 ], [ %apl1.2.i.i22, %encode.exit._crit_edge ]
  %rh1.0 = phi i32 [ 0, %0 ], [ %248, %encode.exit._crit_edge ]
  %plt2.0 = phi i32 [ 0, %0 ], [ %plt1.0, %encode.exit._crit_edge ]
  %plt1.0 = phi i32 [ 0, %0 ], [ %142, %encode.exit._crit_edge ]
  %nbl.0 = phi i32 [ 0, %0 ], [ %...i13.i4, %encode.exit._crit_edge ]
  %detl.0 = phi i32 [ 32, %0 ], [ %141, %encode.exit._crit_edge ]
  %al2.0 = phi i32 [ 0, %0 ], [ %...i11.i8, %encode.exit._crit_edge ]
  %rlt2.0 = phi i32 [ 0, %0 ], [ %rlt1.0, %encode.exit._crit_edge ]
  %al1.0 = phi i32 [ 0, %0 ], [ %apl1.2.i7.i11, %encode.exit._crit_edge ]
  %rlt1.0 = phi i32 [ 0, %0 ], [ %166, %encode.exit._crit_edge ]
  %i.02.i = phi i32 [ 0, %0 ], [ %253, %encode.exit._crit_edge ]
  %27 = getelementptr inbounds [100 x i32]* @test_data, i32 0, i32 %i.02.i
  %28 = load i32* %27, align 4, !tbaa !1
  %29 = or i32 %i.02.i, 1
  %30 = getelementptr inbounds [100 x i32]* @test_data, i32 0, i32 %29
  %31 = load i32* %30, align 4, !tbaa !1
  %32 = mul nsw i32 %26, 12
  %33 = mul nsw i32 %25, -44
  %34 = mul nsw i32 %24, -44
  %35 = add nsw i32 %34, %32
  %36 = mul nsw i32 %23, 212
  %37 = add nsw i32 %36, %33
  %38 = mul nsw i32 %22, 48
  %39 = add nsw i32 %38, %35
  %40 = mul nsw i32 %21, -624
  %41 = add nsw i32 %40, %37
  %42 = shl nsw i32 %20, 7
  %43 = add nsw i32 %42, %39
  %44 = mul nsw i32 %19, 1448
  %45 = add nsw i32 %44, %41
  %46 = mul nsw i32 %18, -840
  %47 = add nsw i32 %46, %43
  %48 = mul nsw i32 %17, -3220
  %49 = add nsw i32 %48, %45
  %50 = mul nsw i32 %16, 3804
  %51 = add nsw i32 %50, %47
  %52 = mul nsw i32 %15, 15504
  %53 = add nsw i32 %52, %49
  %54 = mul nsw i32 %14, 15504
  %55 = add nsw i32 %54, %51
  %56 = mul nsw i32 %13, 3804
  %57 = add nsw i32 %56, %53
  %58 = mul nsw i32 %12, -3220
  %59 = add nsw i32 %58, %55
  %60 = mul nsw i32 %11, -840
  %61 = add nsw i32 %60, %57
  %62 = mul nsw i32 %10, 1448
  %63 = add nsw i32 %62, %59
  %64 = shl nsw i32 %9, 7
  %65 = add nsw i32 %64, %61
  %66 = mul nsw i32 %8, -624
  %67 = add nsw i32 %66, %63
  %68 = mul nsw i32 %7, 48
  %69 = add nsw i32 %68, %65
  %70 = mul nsw i32 %6, 212
  %71 = add nsw i32 %70, %67
  %72 = mul nsw i32 %5, -44
  %73 = add nsw i32 %72, %69
  %74 = mul nsw i32 %4, -44
  %75 = add nsw i32 %74, %71
  %76 = mul nsw i32 %3, 12
  store i32 %5, i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 23), align 4, !tbaa !1
  store i32 %6, i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 22), align 4, !tbaa !1
  store i32 %7, i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 21), align 4, !tbaa !1
  store i32 %8, i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 20), align 4, !tbaa !1
  store i32 %9, i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 19), align 4, !tbaa !1
  store i32 %10, i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 18), align 4, !tbaa !1
  store i32 %11, i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 17), align 4, !tbaa !1
  store i32 %12, i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 16), align 4, !tbaa !1
  store i32 %13, i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 15), align 4, !tbaa !1
  store i32 %14, i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 14), align 4, !tbaa !1
  store i32 %15, i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 13), align 4, !tbaa !1
  store i32 %16, i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 12), align 4, !tbaa !1
  store i32 %17, i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 11), align 4, !tbaa !1
  store i32 %18, i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 10), align 4, !tbaa !1
  store i32 %19, i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 9), align 4, !tbaa !1
  store i32 %20, i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 8), align 4, !tbaa !1
  store i32 %21, i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 7), align 4, !tbaa !1
  store i32 %22, i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 6), align 4, !tbaa !1
  store i32 %23, i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 5), align 4, !tbaa !1
  store i32 %24, i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 4), align 4, !tbaa !1
  store i32 %25, i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 3), align 4, !tbaa !1
  store i32 %26, i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 2), align 4, !tbaa !1
  %77 = add nsw i32 %76, %73
  store i32 %28, i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 1), align 4, !tbaa !1
  store i32 %31, i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 0), align 4, !tbaa !1
  %78 = add nsw i32 %77, %75
  %79 = ashr i32 %78, 15
  %80 = sub nsw i32 %75, %77
  %81 = ashr i32 %80, 15
  %82 = load i32* getelementptr inbounds ([6 x i32]* @delay_bpl, i32 0, i32 0), align 4, !tbaa !1
  %83 = load i32* getelementptr inbounds ([6 x i32]* @delay_dltx, i32 0, i32 0), align 4, !tbaa !1
  %84 = mul nsw i32 %83, %82
  %85 = load i32* getelementptr inbounds ([6 x i32]* @delay_bpl, i32 0, i32 1), align 4, !tbaa !1
  %86 = load i32* getelementptr inbounds ([6 x i32]* @delay_dltx, i32 0, i32 1), align 4, !tbaa !1
  %87 = mul nsw i32 %86, %85
  %88 = add nsw i32 %87, %84
  %89 = load i32* getelementptr inbounds ([6 x i32]* @delay_bpl, i32 0, i32 2), align 4, !tbaa !1
  %90 = load i32* getelementptr inbounds ([6 x i32]* @delay_dltx, i32 0, i32 2), align 4, !tbaa !1
  %91 = mul nsw i32 %90, %89
  %92 = add nsw i32 %88, %91
  %93 = load i32* getelementptr inbounds ([6 x i32]* @delay_bpl, i32 0, i32 3), align 4, !tbaa !1
  %94 = load i32* getelementptr inbounds ([6 x i32]* @delay_dltx, i32 0, i32 3), align 4, !tbaa !1
  %95 = mul nsw i32 %94, %93
  %96 = add nsw i32 %92, %95
  %97 = load i32* getelementptr inbounds ([6 x i32]* @delay_bpl, i32 0, i32 4), align 4, !tbaa !1
  %98 = load i32* getelementptr inbounds ([6 x i32]* @delay_dltx, i32 0, i32 4), align 4, !tbaa !1
  %99 = mul nsw i32 %98, %97
  %100 = add nsw i32 %96, %99
  %101 = load i32* getelementptr inbounds ([6 x i32]* @delay_bpl, i32 0, i32 5), align 4, !tbaa !1
  %102 = load i32* getelementptr inbounds ([6 x i32]* @delay_dltx, i32 0, i32 5), align 4, !tbaa !1
  %103 = mul nsw i32 %102, %101
  %104 = add nsw i32 %100, %103
  %105 = ashr i32 %104, 14
  %106 = mul i32 %al1.0, %rlt1.0
  %107 = mul i32 %al2.0, %rlt2.0
  %tmp.i.i1 = add i32 %107, %106
  %tmp1.i.i2 = shl i32 %tmp.i.i1, 1
  %108 = ashr i32 %tmp1.i.i2, 15
  %109 = add nsw i32 %108, %105
  %110 = sub nsw i32 %79, %109
  %111 = icmp sgt i32 %110, -1
  %112 = sub nsw i32 0, %110
  %m.0.i.i.i = select i1 %111, i32 %110, i32 %112
  br label %115

; <label>:113                                     ; preds = %115
  %114 = icmp slt i32 %121, 30
  br i1 %114, label %115, label %quantl.exit.i

; <label>:115                                     ; preds = %113, %2
  %mil.01.i.i = phi i32 [ 0, %2 ], [ %121, %113 ]
  %116 = getelementptr inbounds [30 x i32]* @decis_levl, i32 0, i32 %mil.01.i.i
  %117 = load i32* %116, align 4, !tbaa !1
  %118 = mul nsw i32 %117, %detl.0
  %119 = ashr i32 %118, 15
  %120 = icmp sgt i32 %m.0.i.i.i, %119
  %121 = add nsw i32 %mil.01.i.i, 1
  br i1 %120, label %113, label %quantl.exit.i

quantl.exit.i:                                    ; preds = %115, %113
  %mil.0.lcssa.i.i = phi i32 [ %mil.01.i.i, %115 ], [ %121, %113 ]
  %quant26bt_pos.quant26bt_neg.i.i = select i1 %111, [31 x i32]* @quant26bt_pos, [31 x i32]* @quant26bt_neg
  %ril.0.in.i.i = getelementptr inbounds [31 x i32]* %quant26bt_pos.quant26bt_neg.i.i, i32 0, i32 %mil.0.lcssa.i.i
  %ril.0.i.i = load i32* %ril.0.in.i.i, align 4
  %122 = ashr i32 %ril.0.i.i, 2
  %123 = getelementptr inbounds [16 x i32]* @qq4_code4_table, i32 0, i32 %122
  %124 = load i32* %123, align 4, !tbaa !1
  %125 = mul nsw i32 %124, %detl.0
  %126 = ashr i32 %125, 15
  %127 = mul nsw i32 %nbl.0, 127
  %128 = ashr i32 %127, 7
  %129 = getelementptr inbounds [16 x i32]* @wl_code_table, i32 0, i32 %122
  %130 = load i32* %129, align 4, !tbaa !1
  %131 = add nsw i32 %128, %130
  %132 = icmp slt i32 %131, 0
  %..i12.i3 = select i1 %132, i32 0, i32 %131
  %133 = icmp sgt i32 %..i12.i3, 18432
  %...i13.i4 = select i1 %133, i32 18432, i32 %..i12.i3
  %134 = lshr i32 %...i13.i4, 6
  %135 = and i32 %134, 31
  %136 = ashr i32 %...i13.i4, 11
  %137 = getelementptr inbounds [32 x i32]* @ilb_table, i32 0, i32 %135
  %138 = load i32* %137, align 4, !tbaa !1
  %139 = sub i32 9, %136
  %140 = ashr i32 %138, %139
  %141 = shl i32 %140, 3
  %142 = add nsw i32 %105, %126
  tail call fastcc void @upzero(i32 %126, i32* getelementptr inbounds ([6 x i32]* @delay_dltx, i32 0, i32 0), i32* getelementptr inbounds ([6 x i32]* @delay_bpl, i32 0, i32 0)) #2
  %143 = shl nsw i32 %al1.0, 2
  %144 = mul nsw i32 %plt1.0, %142
  %145 = icmp sgt i32 %144, -1
  %146 = sub nsw i32 0, %143
  %.1.i8.i5 = select i1 %145, i32 %146, i32 %143
  %147 = ashr i32 %.1.i8.i5, 7
  %148 = mul nsw i32 %plt2.0, %142
  %149 = ashr i32 %148, 31
  %150 = and i32 %149, -256
  %151 = or i32 %150, 128
  %152 = mul nsw i32 %al2.0, 127
  %153 = ashr i32 %152, 7
  %wd4.0.i9.i6 = add nsw i32 %147, %153
  %154 = add nsw i32 %wd4.0.i9.i6, %151
  %155 = icmp sgt i32 %154, 12288
  %..i10.i7 = select i1 %155, i32 12288, i32 %154
  %156 = icmp slt i32 %..i10.i7, -12288
  %...i11.i8 = select i1 %156, i32 -12288, i32 %..i10.i7
  %157 = mul nsw i32 %al1.0, 255
  %158 = ashr i32 %157, 8
  %159 = ashr i32 %144, 31
  %160 = and i32 %159, -384
  %161 = add nsw i32 %158, 192
  %apl1.0.i5.i9 = add nsw i32 %161, %160
  %162 = sub nsw i32 15360, %...i11.i8
  %163 = icmp sgt i32 %apl1.0.i5.i9, %162
  %.apl1.0.i6.i10 = select i1 %163, i32 %162, i32 %apl1.0.i5.i9
  %164 = sub nsw i32 0, %162
  %165 = icmp slt i32 %.apl1.0.i6.i10, %164
  %apl1.2.i7.i11 = select i1 %165, i32 %164, i32 %.apl1.0.i6.i10
  %166 = add nsw i32 %126, %109
  %167 = load i32* getelementptr inbounds ([6 x i32]* @delay_bph, i32 0, i32 0), align 4, !tbaa !1
  %168 = load i32* getelementptr inbounds ([6 x i32]* @delay_dhx, i32 0, i32 0), align 4, !tbaa !1
  %169 = mul nsw i32 %168, %167
  %170 = load i32* getelementptr inbounds ([6 x i32]* @delay_bph, i32 0, i32 1), align 4, !tbaa !1
  %171 = load i32* getelementptr inbounds ([6 x i32]* @delay_dhx, i32 0, i32 1), align 4, !tbaa !1
  %172 = mul nsw i32 %171, %170
  %173 = add nsw i32 %172, %169
  %174 = load i32* getelementptr inbounds ([6 x i32]* @delay_bph, i32 0, i32 2), align 4, !tbaa !1
  %175 = load i32* getelementptr inbounds ([6 x i32]* @delay_dhx, i32 0, i32 2), align 4, !tbaa !1
  %176 = mul nsw i32 %175, %174
  %177 = add nsw i32 %173, %176
  %178 = load i32* getelementptr inbounds ([6 x i32]* @delay_bph, i32 0, i32 3), align 4, !tbaa !1
  %179 = load i32* getelementptr inbounds ([6 x i32]* @delay_dhx, i32 0, i32 3), align 4, !tbaa !1
  %180 = mul nsw i32 %179, %178
  %181 = add nsw i32 %177, %180
  %182 = load i32* getelementptr inbounds ([6 x i32]* @delay_bph, i32 0, i32 4), align 4, !tbaa !1
  %183 = load i32* getelementptr inbounds ([6 x i32]* @delay_dhx, i32 0, i32 4), align 4, !tbaa !1
  %184 = mul nsw i32 %183, %182
  %185 = add nsw i32 %181, %184
  %186 = load i32* getelementptr inbounds ([6 x i32]* @delay_bph, i32 0, i32 5), align 4, !tbaa !1
  %187 = load i32* getelementptr inbounds ([6 x i32]* @delay_dhx, i32 0, i32 5), align 4, !tbaa !1
  %188 = mul nsw i32 %187, %186
  %189 = add nsw i32 %185, %188
  %190 = ashr i32 %189, 14
  %191 = mul i32 %ah1.0, %rh1.0
  %192 = mul i32 %ah2.0, %rh2.0
  %tmp.i3.i12 = add i32 %192, %191
  %tmp1.i4.i13 = shl i32 %tmp.i3.i12, 1
  %193 = ashr i32 %tmp1.i4.i13, 15
  %194 = add nsw i32 %193, %190
  %195 = sub nsw i32 %81, %194
  %196 = ashr i32 %195, 31
  %197 = and i32 %196, -2
  %198 = add nsw i32 %197, 3
  %199 = mul nsw i32 %deth.0, 564
  %200 = ashr i32 %199, 12
  %201 = icmp sgt i32 %195, -1
  %202 = sub nsw i32 0, %195
  %m.0.i.i = select i1 %201, i32 %195, i32 %202
  %203 = icmp sgt i32 %m.0.i.i, %200
  %204 = add nsw i32 %197, 2
  %. = select i1 %203, i32 %204, i32 %198
  %.60 = select i1 %203, i32 %204, i32 %198
  %205 = getelementptr inbounds [4 x i32]* @qq2_code2_table, i32 0, i32 %.60
  %206 = load i32* %205, align 4, !tbaa !1
  %207 = mul nsw i32 %206, %deth.0
  %208 = ashr i32 %207, 15
  %209 = mul nsw i32 %nbh.0, 127
  %210 = ashr i32 %209, 7
  %211 = getelementptr inbounds [4 x i32]* @wh_code_table, i32 0, i32 %.60
  %212 = load i32* %211, align 4, !tbaa !1
  %213 = add nsw i32 %210, %212
  %214 = icmp slt i32 %213, 0
  %..i1.i14 = select i1 %214, i32 0, i32 %213
  %215 = icmp sgt i32 %..i1.i14, 22528
  %...i2.i15 = select i1 %215, i32 22528, i32 %..i1.i14
  %216 = lshr i32 %...i2.i15, 6
  %217 = and i32 %216, 31
  %218 = ashr i32 %...i2.i15, 11
  %219 = getelementptr inbounds [32 x i32]* @ilb_table, i32 0, i32 %217
  %220 = load i32* %219, align 4, !tbaa !1
  %221 = sub i32 11, %218
  %222 = ashr i32 %220, %221
  %223 = shl i32 %222, 3
  %224 = add nsw i32 %190, %208
  tail call fastcc void @upzero(i32 %208, i32* getelementptr inbounds ([6 x i32]* @delay_dhx, i32 0, i32 0), i32* getelementptr inbounds ([6 x i32]* @delay_bph, i32 0, i32 0)) #2
  %225 = shl nsw i32 %ah1.0, 2
  %226 = mul nsw i32 %ph1.0, %224
  %227 = icmp sgt i32 %226, -1
  %228 = sub nsw i32 0, %225
  %.1.i.i16 = select i1 %227, i32 %228, i32 %225
  %229 = ashr i32 %.1.i.i16, 7
  %230 = mul nsw i32 %ph2.0, %224
  %231 = ashr i32 %230, 31
  %232 = and i32 %231, -256
  %233 = or i32 %232, 128
  %234 = mul nsw i32 %ah2.0, 127
  %235 = ashr i32 %234, 7
  %wd4.0.i.i17 = add nsw i32 %229, %235
  %236 = add nsw i32 %wd4.0.i.i17, %233
  %237 = icmp sgt i32 %236, 12288
  %..i.i18 = select i1 %237, i32 12288, i32 %236
  %238 = icmp slt i32 %..i.i18, -12288
  %...i.i19 = select i1 %238, i32 -12288, i32 %..i.i18
  %239 = mul nsw i32 %ah1.0, 255
  %240 = ashr i32 %239, 8
  %241 = ashr i32 %226, 31
  %242 = and i32 %241, -384
  %243 = add nsw i32 %240, 192
  %apl1.0.i.i20 = add nsw i32 %243, %242
  %244 = sub nsw i32 15360, %...i.i19
  %245 = icmp sgt i32 %apl1.0.i.i20, %244
  %.apl1.0.i.i21 = select i1 %245, i32 %244, i32 %apl1.0.i.i20
  %246 = sub nsw i32 0, %244
  %247 = icmp slt i32 %.apl1.0.i.i21, %246
  %apl1.2.i.i22 = select i1 %247, i32 %246, i32 %.apl1.0.i.i21
  %248 = add nsw i32 %208, %194
  %249 = shl i32 %., 6
  %250 = or i32 %249, %ril.0.i.i
  %251 = sdiv i32 %i.02.i, 2
  %252 = getelementptr inbounds [100 x i32]* @compressed, i32 0, i32 %251
  store i32 %250, i32* %252, align 4, !tbaa !1
  %253 = add nsw i32 %i.02.i, 2
  %254 = icmp slt i32 %253, 100
  br i1 %254, label %encode.exit._crit_edge, label %.preheader.i.preheader

encode.exit._crit_edge:                           ; preds = %quantl.exit.i
  %.pre = load i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 0), align 4, !tbaa !1
  %.pre37 = load i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 1), align 4, !tbaa !1
  %.pre38 = load i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 2), align 4, !tbaa !1
  %.pre39 = load i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 3), align 4, !tbaa !1
  %.pre40 = load i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 4), align 4, !tbaa !1
  %.pre41 = load i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 5), align 4, !tbaa !1
  %.pre42 = load i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 6), align 4, !tbaa !1
  %.pre43 = load i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 7), align 4, !tbaa !1
  %.pre44 = load i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 8), align 4, !tbaa !1
  %.pre45 = load i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 9), align 4, !tbaa !1
  %.pre46 = load i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 10), align 4, !tbaa !1
  %.pre47 = load i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 11), align 4, !tbaa !1
  %.pre48 = load i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 12), align 4, !tbaa !1
  %.pre49 = load i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 13), align 4, !tbaa !1
  %.pre50 = load i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 14), align 4, !tbaa !1
  %.pre51 = load i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 15), align 4, !tbaa !1
  %.pre52 = load i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 16), align 4, !tbaa !1
  %.pre53 = load i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 17), align 4, !tbaa !1
  %.pre54 = load i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 18), align 4, !tbaa !1
  %.pre55 = load i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 19), align 4, !tbaa !1
  %.pre56 = load i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 20), align 4, !tbaa !1
  %.pre57 = load i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 21), align 4, !tbaa !1
  %.pre58 = load i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 22), align 4, !tbaa !1
  %.pre59 = load i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 23), align 4, !tbaa !1
  br label %2

.preheader.i.preheader:                           ; preds = %quantl.exit.i
  %ril.0.i.i.lcssa = phi i32 [ %ril.0.i.i, %quantl.exit.i ]
  %255 = getelementptr inbounds [64 x i32]* @qq6_code6_table, i32 0, i32 %ril.0.i.i.lcssa
  %256 = load i32* %255, align 4, !tbaa !1
  br label %.preheader.i

.preheader.i:                                     ; preds = %.preheader.i, %.preheader.i.preheader
  %accumd.10.0 = phi i32 [ %accumd.9.0, %.preheader.i ], [ 0, %.preheader.i.preheader ]
  %accumd.9.0 = phi i32 [ %accumd.8.0, %.preheader.i ], [ 0, %.preheader.i.preheader ]
  %accumd.8.0 = phi i32 [ %accumd.7.0, %.preheader.i ], [ 0, %.preheader.i.preheader ]
  %accumd.7.0 = phi i32 [ %accumd.6.0, %.preheader.i ], [ 0, %.preheader.i.preheader ]
  %accumd.6.0 = phi i32 [ %accumd.5.0, %.preheader.i ], [ 0, %.preheader.i.preheader ]
  %accumd.5.0 = phi i32 [ %accumd.4.0, %.preheader.i ], [ 0, %.preheader.i.preheader ]
  %accumd.4.0 = phi i32 [ %accumd.3.0, %.preheader.i ], [ 0, %.preheader.i.preheader ]
  %accumd.3.0 = phi i32 [ %accumd.2.0, %.preheader.i ], [ 0, %.preheader.i.preheader ]
  %accumd.2.0 = phi i32 [ %accumd.1.0, %.preheader.i ], [ 0, %.preheader.i.preheader ]
  %accumd.1.0 = phi i32 [ %accumd.0.0, %.preheader.i ], [ 0, %.preheader.i.preheader ]
  %accumd.0.0 = phi i32 [ %411, %.preheader.i ], [ 0, %.preheader.i.preheader ]
  %accumc.10.0 = phi i32 [ %accumc.9.0, %.preheader.i ], [ 0, %.preheader.i.preheader ]
  %accumc.9.0 = phi i32 [ %accumc.8.0, %.preheader.i ], [ 0, %.preheader.i.preheader ]
  %accumc.8.0 = phi i32 [ %accumc.7.0, %.preheader.i ], [ 0, %.preheader.i.preheader ]
  %accumc.7.0 = phi i32 [ %accumc.6.0, %.preheader.i ], [ 0, %.preheader.i.preheader ]
  %accumc.6.0 = phi i32 [ %accumc.5.0, %.preheader.i ], [ 0, %.preheader.i.preheader ]
  %accumc.5.0 = phi i32 [ %accumc.4.0, %.preheader.i ], [ 0, %.preheader.i.preheader ]
  %accumc.4.0 = phi i32 [ %accumc.3.0, %.preheader.i ], [ 0, %.preheader.i.preheader ]
  %accumc.3.0 = phi i32 [ %accumc.2.0, %.preheader.i ], [ 0, %.preheader.i.preheader ]
  %accumc.2.0 = phi i32 [ %accumc.1.0, %.preheader.i ], [ 0, %.preheader.i.preheader ]
  %accumc.1.0 = phi i32 [ %accumc.0.0, %.preheader.i ], [ 0, %.preheader.i.preheader ]
  %accumc.0.0 = phi i32 [ %410, %.preheader.i ], [ 0, %.preheader.i.preheader ]
  %dec_ph2.0 = phi i32 [ %dec_ph1.0, %.preheader.i ], [ 0, %.preheader.i.preheader ]
  %dec_ph1.0 = phi i32 [ %385, %.preheader.i ], [ 0, %.preheader.i.preheader ]
  %dec_nbh.0 = phi i32 [ %...i2.i, %.preheader.i ], [ 0, %.preheader.i.preheader ]
  %dec_deth.0 = phi i32 [ %384, %.preheader.i ], [ 8, %.preheader.i.preheader ]
  %dec_ah2.0 = phi i32 [ %...i.i, %.preheader.i ], [ 0, %.preheader.i.preheader ]
  %dec_rh2.0 = phi i32 [ %dec_rh1.0, %.preheader.i ], [ 0, %.preheader.i.preheader ]
  %dec_ah1.0 = phi i32 [ %apl1.2.i.i, %.preheader.i ], [ 0, %.preheader.i.preheader ]
  %dec_rh1.0 = phi i32 [ %409, %.preheader.i ], [ 0, %.preheader.i.preheader ]
  %dec_plt2.0 = phi i32 [ %dec_plt1.0, %.preheader.i ], [ 0, %.preheader.i.preheader ]
  %dec_plt1.0 = phi i32 [ %313, %.preheader.i ], [ 0, %.preheader.i.preheader ]
  %dec_nbl.0 = phi i32 [ %...i13.i, %.preheader.i ], [ 0, %.preheader.i.preheader ]
  %dec_detl.0 = phi i32 [ %312, %.preheader.i ], [ 32, %.preheader.i.preheader ]
  %dec_al2.0 = phi i32 [ %...i11.i, %.preheader.i ], [ 0, %.preheader.i.preheader ]
  %dec_rlt2.0 = phi i32 [ %dec_rlt1.0, %.preheader.i ], [ 0, %.preheader.i.preheader ]
  %dec_al1.0 = phi i32 [ %apl1.2.i7.i, %.preheader.i ], [ 0, %.preheader.i.preheader ]
  %dec_rlt1.0 = phi i32 [ %337, %.preheader.i ], [ 0, %.preheader.i.preheader ]
  %i.11.i = phi i32 [ %463, %.preheader.i ], [ 0, %.preheader.i.preheader ]
  %257 = sdiv i32 %i.11.i, 2
  %258 = getelementptr inbounds [100 x i32]* @compressed, i32 0, i32 %257
  %259 = load i32* %258, align 4, !tbaa !1
  %260 = and i32 %259, 63
  %261 = ashr i32 %259, 6
  %262 = load i32* getelementptr inbounds ([6 x i32]* @dec_del_bpl, i32 0, i32 0), align 4, !tbaa !1
  %263 = load i32* getelementptr inbounds ([6 x i32]* @dec_del_dltx, i32 0, i32 0), align 4, !tbaa !1
  %264 = mul nsw i32 %263, %262
  %265 = load i32* getelementptr inbounds ([6 x i32]* @dec_del_bpl, i32 0, i32 1), align 4, !tbaa !1
  %266 = load i32* getelementptr inbounds ([6 x i32]* @dec_del_dltx, i32 0, i32 1), align 4, !tbaa !1
  %267 = mul nsw i32 %266, %265
  %268 = add nsw i32 %267, %264
  %269 = load i32* getelementptr inbounds ([6 x i32]* @dec_del_bpl, i32 0, i32 2), align 4, !tbaa !1
  %270 = load i32* getelementptr inbounds ([6 x i32]* @dec_del_dltx, i32 0, i32 2), align 4, !tbaa !1
  %271 = mul nsw i32 %270, %269
  %272 = add nsw i32 %268, %271
  %273 = load i32* getelementptr inbounds ([6 x i32]* @dec_del_bpl, i32 0, i32 3), align 4, !tbaa !1
  %274 = load i32* getelementptr inbounds ([6 x i32]* @dec_del_dltx, i32 0, i32 3), align 4, !tbaa !1
  %275 = mul nsw i32 %274, %273
  %276 = add nsw i32 %272, %275
  %277 = load i32* getelementptr inbounds ([6 x i32]* @dec_del_bpl, i32 0, i32 4), align 4, !tbaa !1
  %278 = load i32* getelementptr inbounds ([6 x i32]* @dec_del_dltx, i32 0, i32 4), align 4, !tbaa !1
  %279 = mul nsw i32 %278, %277
  %280 = add nsw i32 %276, %279
  %281 = load i32* getelementptr inbounds ([6 x i32]* @dec_del_bpl, i32 0, i32 5), align 4, !tbaa !1
  %282 = load i32* getelementptr inbounds ([6 x i32]* @dec_del_dltx, i32 0, i32 5), align 4, !tbaa !1
  %283 = mul nsw i32 %282, %281
  %284 = add nsw i32 %280, %283
  %285 = ashr i32 %284, 14
  %286 = mul i32 %dec_al1.0, %dec_rlt1.0
  %287 = mul i32 %dec_al2.0, %dec_rlt2.0
  %tmp.i.i = add i32 %287, %286
  %tmp1.i.i = shl i32 %tmp.i.i, 1
  %288 = ashr i32 %tmp1.i.i, 15
  %289 = add nsw i32 %288, %285
  %290 = lshr i32 %260, 2
  %291 = getelementptr inbounds [16 x i32]* @qq4_code4_table, i32 0, i32 %290
  %292 = load i32* %291, align 4, !tbaa !1
  %293 = mul nsw i32 %292, %dec_detl.0
  %294 = ashr i32 %293, 15
  %295 = mul nsw i32 %256, %dec_detl.0
  %296 = ashr i32 %295, 15
  %297 = add nsw i32 %296, %289
  %298 = mul nsw i32 %dec_nbl.0, 127
  %299 = ashr i32 %298, 7
  %300 = getelementptr inbounds [16 x i32]* @wl_code_table, i32 0, i32 %290
  %301 = load i32* %300, align 4, !tbaa !1
  %302 = add nsw i32 %299, %301
  %303 = icmp slt i32 %302, 0
  %..i12.i = select i1 %303, i32 0, i32 %302
  %304 = icmp sgt i32 %..i12.i, 18432
  %...i13.i = select i1 %304, i32 18432, i32 %..i12.i
  %305 = lshr i32 %...i13.i, 6
  %306 = and i32 %305, 31
  %307 = ashr i32 %...i13.i, 11
  %308 = getelementptr inbounds [32 x i32]* @ilb_table, i32 0, i32 %306
  %309 = load i32* %308, align 4, !tbaa !1
  %310 = sub i32 9, %307
  %311 = ashr i32 %309, %310
  %312 = shl i32 %311, 3
  %313 = add nsw i32 %285, %294
  tail call fastcc void @upzero(i32 %294, i32* getelementptr inbounds ([6 x i32]* @dec_del_dltx, i32 0, i32 0), i32* getelementptr inbounds ([6 x i32]* @dec_del_bpl, i32 0, i32 0)) #2
  %314 = shl nsw i32 %dec_al1.0, 2
  %315 = mul nsw i32 %dec_plt1.0, %313
  %316 = icmp sgt i32 %315, -1
  %317 = sub nsw i32 0, %314
  %.1.i8.i = select i1 %316, i32 %317, i32 %314
  %318 = ashr i32 %.1.i8.i, 7
  %319 = mul nsw i32 %dec_plt2.0, %313
  %320 = ashr i32 %319, 31
  %321 = and i32 %320, -256
  %322 = or i32 %321, 128
  %323 = mul nsw i32 %dec_al2.0, 127
  %324 = ashr i32 %323, 7
  %wd4.0.i9.i = add nsw i32 %318, %324
  %325 = add nsw i32 %wd4.0.i9.i, %322
  %326 = icmp sgt i32 %325, 12288
  %..i10.i = select i1 %326, i32 12288, i32 %325
  %327 = icmp slt i32 %..i10.i, -12288
  %...i11.i = select i1 %327, i32 -12288, i32 %..i10.i
  %328 = mul nsw i32 %dec_al1.0, 255
  %329 = ashr i32 %328, 8
  %330 = ashr i32 %315, 31
  %331 = and i32 %330, -384
  %332 = add nsw i32 %329, 192
  %apl1.0.i5.i = add nsw i32 %332, %331
  %333 = sub nsw i32 15360, %...i11.i
  %334 = icmp sgt i32 %apl1.0.i5.i, %333
  %.apl1.0.i6.i = select i1 %334, i32 %333, i32 %apl1.0.i5.i
  %335 = sub nsw i32 0, %333
  %336 = icmp slt i32 %.apl1.0.i6.i, %335
  %apl1.2.i7.i = select i1 %336, i32 %335, i32 %.apl1.0.i6.i
  %337 = add nsw i32 %294, %289
  %338 = load i32* getelementptr inbounds ([6 x i32]* @dec_del_bph, i32 0, i32 0), align 4, !tbaa !1
  %339 = load i32* getelementptr inbounds ([6 x i32]* @dec_del_dhx, i32 0, i32 0), align 4, !tbaa !1
  %340 = mul nsw i32 %339, %338
  %341 = load i32* getelementptr inbounds ([6 x i32]* @dec_del_bph, i32 0, i32 1), align 4, !tbaa !1
  %342 = load i32* getelementptr inbounds ([6 x i32]* @dec_del_dhx, i32 0, i32 1), align 4, !tbaa !1
  %343 = mul nsw i32 %342, %341
  %344 = add nsw i32 %343, %340
  %345 = load i32* getelementptr inbounds ([6 x i32]* @dec_del_bph, i32 0, i32 2), align 4, !tbaa !1
  %346 = load i32* getelementptr inbounds ([6 x i32]* @dec_del_dhx, i32 0, i32 2), align 4, !tbaa !1
  %347 = mul nsw i32 %346, %345
  %348 = add nsw i32 %344, %347
  %349 = load i32* getelementptr inbounds ([6 x i32]* @dec_del_bph, i32 0, i32 3), align 4, !tbaa !1
  %350 = load i32* getelementptr inbounds ([6 x i32]* @dec_del_dhx, i32 0, i32 3), align 4, !tbaa !1
  %351 = mul nsw i32 %350, %349
  %352 = add nsw i32 %348, %351
  %353 = load i32* getelementptr inbounds ([6 x i32]* @dec_del_bph, i32 0, i32 4), align 4, !tbaa !1
  %354 = load i32* getelementptr inbounds ([6 x i32]* @dec_del_dhx, i32 0, i32 4), align 4, !tbaa !1
  %355 = mul nsw i32 %354, %353
  %356 = add nsw i32 %352, %355
  %357 = load i32* getelementptr inbounds ([6 x i32]* @dec_del_bph, i32 0, i32 5), align 4, !tbaa !1
  %358 = load i32* getelementptr inbounds ([6 x i32]* @dec_del_dhx, i32 0, i32 5), align 4, !tbaa !1
  %359 = mul nsw i32 %358, %357
  %360 = add nsw i32 %356, %359
  %361 = ashr i32 %360, 14
  %362 = mul i32 %dec_ah1.0, %dec_rh1.0
  %363 = mul i32 %dec_ah2.0, %dec_rh2.0
  %tmp.i3.i = add i32 %363, %362
  %tmp1.i4.i = shl i32 %tmp.i3.i, 1
  %364 = ashr i32 %tmp1.i4.i, 15
  %365 = add nsw i32 %364, %361
  %366 = getelementptr inbounds [4 x i32]* @qq2_code2_table, i32 0, i32 %261
  %367 = load i32* %366, align 4, !tbaa !1
  %368 = mul nsw i32 %367, %dec_deth.0
  %369 = ashr i32 %368, 15
  %370 = mul nsw i32 %dec_nbh.0, 127
  %371 = ashr i32 %370, 7
  %372 = getelementptr inbounds [4 x i32]* @wh_code_table, i32 0, i32 %261
  %373 = load i32* %372, align 4, !tbaa !1
  %374 = add nsw i32 %371, %373
  %375 = icmp slt i32 %374, 0
  %..i1.i = select i1 %375, i32 0, i32 %374
  %376 = icmp sgt i32 %..i1.i, 22528
  %...i2.i = select i1 %376, i32 22528, i32 %..i1.i
  %377 = lshr i32 %...i2.i, 6
  %378 = and i32 %377, 31
  %379 = ashr i32 %...i2.i, 11
  %380 = getelementptr inbounds [32 x i32]* @ilb_table, i32 0, i32 %378
  %381 = load i32* %380, align 4, !tbaa !1
  %382 = sub i32 11, %379
  %383 = ashr i32 %381, %382
  %384 = shl i32 %383, 3
  %385 = add nsw i32 %361, %369
  tail call fastcc void @upzero(i32 %369, i32* getelementptr inbounds ([6 x i32]* @dec_del_dhx, i32 0, i32 0), i32* getelementptr inbounds ([6 x i32]* @dec_del_bph, i32 0, i32 0)) #2
  %386 = shl nsw i32 %dec_ah1.0, 2
  %387 = mul nsw i32 %dec_ph1.0, %385
  %388 = icmp sgt i32 %387, -1
  %389 = sub nsw i32 0, %386
  %.1.i.i = select i1 %388, i32 %389, i32 %386
  %390 = ashr i32 %.1.i.i, 7
  %391 = mul nsw i32 %dec_ph2.0, %385
  %392 = ashr i32 %391, 31
  %393 = and i32 %392, -256
  %394 = or i32 %393, 128
  %395 = mul nsw i32 %dec_ah2.0, 127
  %396 = ashr i32 %395, 7
  %wd4.0.i.i = add nsw i32 %390, %396
  %397 = add nsw i32 %wd4.0.i.i, %394
  %398 = icmp sgt i32 %397, 12288
  %..i.i = select i1 %398, i32 12288, i32 %397
  %399 = icmp slt i32 %..i.i, -12288
  %...i.i = select i1 %399, i32 -12288, i32 %..i.i
  %400 = mul nsw i32 %dec_ah1.0, 255
  %401 = ashr i32 %400, 8
  %402 = ashr i32 %387, 31
  %403 = and i32 %402, -384
  %404 = add nsw i32 %401, 192
  %apl1.0.i.i = add nsw i32 %404, %403
  %405 = sub nsw i32 15360, %...i.i
  %406 = icmp sgt i32 %apl1.0.i.i, %405
  %.apl1.0.i.i = select i1 %406, i32 %405, i32 %apl1.0.i.i
  %407 = sub nsw i32 0, %405
  %408 = icmp slt i32 %.apl1.0.i.i, %407
  %apl1.2.i.i = select i1 %408, i32 %407, i32 %.apl1.0.i.i
  %409 = add nsw i32 %369, %365
  %410 = sub nsw i32 %297, %409
  %411 = add nsw i32 %409, %297
  %412 = mul nsw i32 %410, 12
  %413 = mul nsw i32 %411, -44
  %414 = mul nsw i32 %accumc.0.0, -44
  %415 = add nsw i32 %414, %412
  %416 = mul nsw i32 %accumd.0.0, 212
  %417 = add nsw i32 %416, %413
  %418 = mul nsw i32 %accumc.1.0, 48
  %419 = add nsw i32 %418, %415
  %420 = mul nsw i32 %accumd.1.0, -624
  %421 = add nsw i32 %420, %417
  %422 = shl nsw i32 %accumc.2.0, 7
  %423 = add nsw i32 %422, %419
  %424 = mul nsw i32 %accumd.2.0, 1448
  %425 = add nsw i32 %424, %421
  %426 = mul nsw i32 %accumc.3.0, -840
  %427 = add nsw i32 %426, %423
  %428 = mul nsw i32 %accumd.3.0, -3220
  %429 = add nsw i32 %428, %425
  %430 = mul nsw i32 %accumc.4.0, 3804
  %431 = add nsw i32 %430, %427
  %432 = mul nsw i32 %accumd.4.0, 15504
  %433 = add nsw i32 %432, %429
  %434 = mul nsw i32 %accumc.5.0, 15504
  %435 = add nsw i32 %434, %431
  %436 = mul nsw i32 %accumd.5.0, 3804
  %437 = add nsw i32 %436, %433
  %438 = mul nsw i32 %accumc.6.0, -3220
  %439 = add nsw i32 %438, %435
  %440 = mul nsw i32 %accumd.6.0, -840
  %441 = add nsw i32 %440, %437
  %442 = mul nsw i32 %accumc.7.0, 1448
  %443 = add nsw i32 %442, %439
  %444 = shl nsw i32 %accumd.7.0, 7
  %445 = add nsw i32 %444, %441
  %446 = mul nsw i32 %accumc.8.0, -624
  %447 = add nsw i32 %446, %443
  %448 = mul nsw i32 %accumd.8.0, 48
  %449 = add nsw i32 %448, %445
  %450 = mul nsw i32 %accumc.9.0, 212
  %451 = add nsw i32 %450, %447
  %452 = mul nsw i32 %accumd.9.0, -44
  %453 = add nsw i32 %452, %449
  %454 = mul nsw i32 %accumc.10.0, -44
  %455 = add nsw i32 %454, %451
  %456 = mul nsw i32 %accumd.10.0, 12
  %457 = add nsw i32 %456, %453
  %458 = ashr i32 %455, 14
  %459 = ashr i32 %457, 14
  %460 = getelementptr inbounds [100 x i32]* @result, i32 0, i32 %i.11.i
  store i32 %458, i32* %460, align 4, !tbaa !1
  %461 = or i32 %i.11.i, 1
  %462 = getelementptr inbounds [100 x i32]* @result, i32 0, i32 %461
  store i32 %459, i32* %462, align 4, !tbaa !1
  %463 = add nsw i32 %i.11.i, 2
  %464 = icmp slt i32 %463, 100
  br i1 %464, label %.preheader.i, label %adpcm_main.exit

adpcm_main.exit:                                  ; preds = %adpcm_main.exit, %.preheader.i
  %main_result.04 = phi i32 [ %.main_result.0, %adpcm_main.exit ], [ 0, %.preheader.i ]
  %i.03 = phi i32 [ %471, %adpcm_main.exit ], [ 0, %.preheader.i ]
  %465 = getelementptr inbounds [100 x i32]* @compressed, i32 0, i32 %i.03
  %466 = load i32* %465, align 4, !tbaa !1
  %467 = getelementptr inbounds [100 x i32]* @test_compressed, i32 0, i32 %i.03
  %468 = load i32* %467, align 4, !tbaa !1
  %469 = icmp eq i32 %466, %468
  %470 = zext i1 %469 to i32
  %.main_result.0 = add nsw i32 %470, %main_result.04
  %471 = add nsw i32 %i.03, 1
  %exitcond5 = icmp eq i32 %471, 50
  br i1 %exitcond5, label %.preheader, label %adpcm_main.exit

.preheader:                                       ; preds = %.preheader, %adpcm_main.exit
  %main_result.22 = phi i32 [ %.main_result.2, %.preheader ], [ %.main_result.0, %adpcm_main.exit ]
  %i.11 = phi i32 [ %478, %.preheader ], [ 0, %adpcm_main.exit ]
  %472 = getelementptr inbounds [100 x i32]* @result, i32 0, i32 %i.11
  %473 = load i32* %472, align 4, !tbaa !1
  %474 = getelementptr inbounds [100 x i32]* @test_result, i32 0, i32 %i.11
  %475 = load i32* %474, align 4, !tbaa !1
  %476 = icmp eq i32 %473, %475
  %477 = zext i1 %476 to i32
  %.main_result.2 = add nsw i32 %477, %main_result.22
  %478 = add nsw i32 %i.11, 1
  %exitcond = icmp eq i32 %478, 100
  br i1 %exitcond, label %479, label %.preheader

; <label>:479                                     ; preds = %.preheader
  %.main_result.2.lcssa = phi i32 [ %.main_result.2, %.preheader ]
  %480 = tail call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([12 x i8]* @.str, i32 0, i32 0), i32 %.main_result.2.lcssa) #2
  %481 = icmp eq i32 %.main_result.2.lcssa, 150
  br i1 %481, label %482, label %484

; <label>:482                                     ; preds = %479
  %483 = tail call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([14 x i8]* @.str1, i32 0, i32 0)) #2
  br label %486

; <label>:484                                     ; preds = %479
  %485 = tail call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([14 x i8]* @.str2, i32 0, i32 0)) #2
  br label %486

; <label>:486                                     ; preds = %484, %482
  ret i32 %.main_result.2.lcssa
}

; Function Attrs: nounwind
declare i32 @printf(i8* nocapture readonly, ...) #0

; Function Attrs: nounwind
declare void @llvm.memset.p0i8.i64(i8* nocapture, i8, i64, i32, i1) #1

declare void @legup_memset_4_i64(i8*, i8, i64)

attributes #0 = { nounwind "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind }
attributes #2 = { nobuiltin nounwind }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"clang version 3.5.0 (tags/RELEASE_350/final)"}
!1 = metadata !{metadata !2, metadata !2, i64 0}
!2 = metadata !{metadata !"int", metadata !3, i64 0}
!3 = metadata !{metadata !"omnipotent char", metadata !4, i64 0}
!4 = metadata !{metadata !"Simple C/C++ TBAA"}
