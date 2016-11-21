; ModuleID = 'adpcm.postlto.8.bc'
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
  call void @legup_memset_4_i64(i8* bitcast ([24 x i32]* @tqmf to i8*), i8 0, i64 96)
  br label %1

; <label>:1                                       ; preds = %encode.exit._crit_edge, %0
  %2 = phi i32 [ 0, %0 ], [ %.pre59, %encode.exit._crit_edge ]
  %3 = phi i32 [ 0, %0 ], [ %.pre58, %encode.exit._crit_edge ]
  %4 = phi i32 [ 0, %0 ], [ %.pre57, %encode.exit._crit_edge ]
  %5 = phi i32 [ 0, %0 ], [ %.pre56, %encode.exit._crit_edge ]
  %6 = phi i32 [ 0, %0 ], [ %.pre55, %encode.exit._crit_edge ]
  %7 = phi i32 [ 0, %0 ], [ %.pre54, %encode.exit._crit_edge ]
  %8 = phi i32 [ 0, %0 ], [ %.pre53, %encode.exit._crit_edge ]
  %9 = phi i32 [ 0, %0 ], [ %.pre52, %encode.exit._crit_edge ]
  %10 = phi i32 [ 0, %0 ], [ %.pre51, %encode.exit._crit_edge ]
  %11 = phi i32 [ 0, %0 ], [ %.pre50, %encode.exit._crit_edge ]
  %12 = phi i32 [ 0, %0 ], [ %.pre49, %encode.exit._crit_edge ]
  %13 = phi i32 [ 0, %0 ], [ %.pre48, %encode.exit._crit_edge ]
  %14 = phi i32 [ 0, %0 ], [ %.pre47, %encode.exit._crit_edge ]
  %15 = phi i32 [ 0, %0 ], [ %.pre46, %encode.exit._crit_edge ]
  %16 = phi i32 [ 0, %0 ], [ %.pre45, %encode.exit._crit_edge ]
  %17 = phi i32 [ 0, %0 ], [ %.pre44, %encode.exit._crit_edge ]
  %18 = phi i32 [ 0, %0 ], [ %.pre43, %encode.exit._crit_edge ]
  %19 = phi i32 [ 0, %0 ], [ %.pre42, %encode.exit._crit_edge ]
  %20 = phi i32 [ 0, %0 ], [ %.pre41, %encode.exit._crit_edge ]
  %21 = phi i32 [ 0, %0 ], [ %.pre40, %encode.exit._crit_edge ]
  %22 = phi i32 [ 0, %0 ], [ %.pre39, %encode.exit._crit_edge ]
  %23 = phi i32 [ 0, %0 ], [ %.pre38, %encode.exit._crit_edge ]
  %24 = phi i32 [ 0, %0 ], [ %.pre37, %encode.exit._crit_edge ]
  %25 = phi i32 [ 0, %0 ], [ %.pre, %encode.exit._crit_edge ]
  %ph2.0 = phi i32 [ 0, %0 ], [ %ph1.0, %encode.exit._crit_edge ]
  %ph1.0 = phi i32 [ 0, %0 ], [ %223, %encode.exit._crit_edge ]
  %nbh.0 = phi i32 [ 0, %0 ], [ %...i2.i15, %encode.exit._crit_edge ]
  %deth.0 = phi i32 [ 8, %0 ], [ %222, %encode.exit._crit_edge ]
  %ah2.0 = phi i32 [ 0, %0 ], [ %...i.i19, %encode.exit._crit_edge ]
  %rh2.0 = phi i32 [ 0, %0 ], [ %rh1.0, %encode.exit._crit_edge ]
  %ah1.0 = phi i32 [ 0, %0 ], [ %apl1.2.i.i22, %encode.exit._crit_edge ]
  %rh1.0 = phi i32 [ 0, %0 ], [ %247, %encode.exit._crit_edge ]
  %plt2.0 = phi i32 [ 0, %0 ], [ %plt1.0, %encode.exit._crit_edge ]
  %plt1.0 = phi i32 [ 0, %0 ], [ %141, %encode.exit._crit_edge ]
  %nbl.0 = phi i32 [ 0, %0 ], [ %...i13.i4, %encode.exit._crit_edge ]
  %detl.0 = phi i32 [ 32, %0 ], [ %140, %encode.exit._crit_edge ]
  %al2.0 = phi i32 [ 0, %0 ], [ %...i11.i8, %encode.exit._crit_edge ]
  %rlt2.0 = phi i32 [ 0, %0 ], [ %rlt1.0, %encode.exit._crit_edge ]
  %al1.0 = phi i32 [ 0, %0 ], [ %apl1.2.i7.i11, %encode.exit._crit_edge ]
  %rlt1.0 = phi i32 [ 0, %0 ], [ %165, %encode.exit._crit_edge ]
  %i.02.i = phi i32 [ 0, %0 ], [ %252, %encode.exit._crit_edge ]
  %26 = getelementptr inbounds [100 x i32]* @test_data, i32 0, i32 %i.02.i
  %27 = load i32* %26, align 4, !tbaa !1
  %28 = or i32 %i.02.i, 1
  %29 = getelementptr inbounds [100 x i32]* @test_data, i32 0, i32 %28
  %30 = load i32* %29, align 4, !tbaa !1
  %31 = mul nsw i32 %25, 12
  %32 = mul nsw i32 %24, -44
  %33 = mul nsw i32 %23, -44
  %34 = add nsw i32 %33, %31
  %35 = mul nsw i32 %22, 212
  %36 = add nsw i32 %35, %32
  %37 = mul nsw i32 %21, 48
  %38 = add nsw i32 %37, %34
  %39 = mul nsw i32 %20, -624
  %40 = add nsw i32 %39, %36
  %41 = shl nsw i32 %19, 7
  %42 = add nsw i32 %41, %38
  %43 = mul nsw i32 %18, 1448
  %44 = add nsw i32 %43, %40
  %45 = mul nsw i32 %17, -840
  %46 = add nsw i32 %45, %42
  %47 = mul nsw i32 %16, -3220
  %48 = add nsw i32 %47, %44
  %49 = mul nsw i32 %15, 3804
  %50 = add nsw i32 %49, %46
  %51 = mul nsw i32 %14, 15504
  %52 = add nsw i32 %51, %48
  %53 = mul nsw i32 %13, 15504
  %54 = add nsw i32 %53, %50
  %55 = mul nsw i32 %12, 3804
  %56 = add nsw i32 %55, %52
  %57 = mul nsw i32 %11, -3220
  %58 = add nsw i32 %57, %54
  %59 = mul nsw i32 %10, -840
  %60 = add nsw i32 %59, %56
  %61 = mul nsw i32 %9, 1448
  %62 = add nsw i32 %61, %58
  %63 = shl nsw i32 %8, 7
  %64 = add nsw i32 %63, %60
  %65 = mul nsw i32 %7, -624
  %66 = add nsw i32 %65, %62
  %67 = mul nsw i32 %6, 48
  %68 = add nsw i32 %67, %64
  %69 = mul nsw i32 %5, 212
  %70 = add nsw i32 %69, %66
  %71 = mul nsw i32 %4, -44
  %72 = add nsw i32 %71, %68
  %73 = mul nsw i32 %3, -44
  %74 = add nsw i32 %73, %70
  %75 = mul nsw i32 %2, 12
  store i32 %4, i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 23), align 4, !tbaa !1
  store i32 %5, i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 22), align 4, !tbaa !1
  store i32 %6, i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 21), align 4, !tbaa !1
  store i32 %7, i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 20), align 4, !tbaa !1
  store i32 %8, i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 19), align 4, !tbaa !1
  store i32 %9, i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 18), align 4, !tbaa !1
  store i32 %10, i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 17), align 4, !tbaa !1
  store i32 %11, i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 16), align 4, !tbaa !1
  store i32 %12, i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 15), align 4, !tbaa !1
  store i32 %13, i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 14), align 4, !tbaa !1
  store i32 %14, i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 13), align 4, !tbaa !1
  store i32 %15, i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 12), align 4, !tbaa !1
  store i32 %16, i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 11), align 4, !tbaa !1
  store i32 %17, i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 10), align 4, !tbaa !1
  store i32 %18, i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 9), align 4, !tbaa !1
  store i32 %19, i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 8), align 4, !tbaa !1
  store i32 %20, i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 7), align 4, !tbaa !1
  store i32 %21, i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 6), align 4, !tbaa !1
  store i32 %22, i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 5), align 4, !tbaa !1
  store i32 %23, i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 4), align 4, !tbaa !1
  store i32 %24, i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 3), align 4, !tbaa !1
  store i32 %25, i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 2), align 4, !tbaa !1
  %76 = add nsw i32 %75, %72
  store i32 %27, i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 1), align 4, !tbaa !1
  store i32 %30, i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 0), align 4, !tbaa !1
  %77 = add nsw i32 %76, %74
  %78 = ashr i32 %77, 15
  %79 = sub nsw i32 %74, %76
  %80 = ashr i32 %79, 15
  %81 = load i32* getelementptr inbounds ([6 x i32]* @delay_bpl, i32 0, i32 0), align 4, !tbaa !1
  %82 = load i32* getelementptr inbounds ([6 x i32]* @delay_dltx, i32 0, i32 0), align 4, !tbaa !1
  %83 = mul nsw i32 %82, %81
  %84 = load i32* getelementptr inbounds ([6 x i32]* @delay_bpl, i32 0, i32 1), align 4, !tbaa !1
  %85 = load i32* getelementptr inbounds ([6 x i32]* @delay_dltx, i32 0, i32 1), align 4, !tbaa !1
  %86 = mul nsw i32 %85, %84
  %87 = add nsw i32 %86, %83
  %88 = load i32* getelementptr inbounds ([6 x i32]* @delay_bpl, i32 0, i32 2), align 4, !tbaa !1
  %89 = load i32* getelementptr inbounds ([6 x i32]* @delay_dltx, i32 0, i32 2), align 4, !tbaa !1
  %90 = mul nsw i32 %89, %88
  %91 = add nsw i32 %87, %90
  %92 = load i32* getelementptr inbounds ([6 x i32]* @delay_bpl, i32 0, i32 3), align 4, !tbaa !1
  %93 = load i32* getelementptr inbounds ([6 x i32]* @delay_dltx, i32 0, i32 3), align 4, !tbaa !1
  %94 = mul nsw i32 %93, %92
  %95 = add nsw i32 %91, %94
  %96 = load i32* getelementptr inbounds ([6 x i32]* @delay_bpl, i32 0, i32 4), align 4, !tbaa !1
  %97 = load i32* getelementptr inbounds ([6 x i32]* @delay_dltx, i32 0, i32 4), align 4, !tbaa !1
  %98 = mul nsw i32 %97, %96
  %99 = add nsw i32 %95, %98
  %100 = load i32* getelementptr inbounds ([6 x i32]* @delay_bpl, i32 0, i32 5), align 4, !tbaa !1
  %101 = load i32* getelementptr inbounds ([6 x i32]* @delay_dltx, i32 0, i32 5), align 4, !tbaa !1
  %102 = mul nsw i32 %101, %100
  %103 = add nsw i32 %99, %102
  %104 = ashr i32 %103, 14
  %105 = mul i32 %al1.0, %rlt1.0
  %106 = mul i32 %al2.0, %rlt2.0
  %tmp.i.i1 = add i32 %106, %105
  %tmp1.i.i2 = shl i32 %tmp.i.i1, 1
  %107 = ashr i32 %tmp1.i.i2, 15
  %108 = add nsw i32 %107, %104
  %109 = sub nsw i32 %78, %108
  %110 = icmp sgt i32 %109, -1
  %111 = sub nsw i32 0, %109
  %m.0.i.i.i = select i1 %110, i32 %109, i32 %111
  br label %114

; <label>:112                                     ; preds = %114
  %113 = icmp slt i32 %120, 30
  br i1 %113, label %114, label %quantl.exit.i

; <label>:114                                     ; preds = %112, %1
  %mil.01.i.i = phi i32 [ 0, %1 ], [ %120, %112 ]
  %115 = getelementptr inbounds [30 x i32]* @decis_levl, i32 0, i32 %mil.01.i.i
  %116 = load i32* %115, align 4, !tbaa !1
  %117 = mul nsw i32 %116, %detl.0
  %118 = ashr i32 %117, 15
  %119 = icmp sgt i32 %m.0.i.i.i, %118
  %120 = add nsw i32 %mil.01.i.i, 1
  br i1 %119, label %112, label %quantl.exit.i

quantl.exit.i:                                    ; preds = %114, %112
  %mil.0.lcssa.i.i = phi i32 [ %mil.01.i.i, %114 ], [ %120, %112 ]
  %quant26bt_pos.quant26bt_neg.i.i = select i1 %110, [31 x i32]* @quant26bt_pos, [31 x i32]* @quant26bt_neg
  %ril.0.in.i.i = getelementptr inbounds [31 x i32]* %quant26bt_pos.quant26bt_neg.i.i, i32 0, i32 %mil.0.lcssa.i.i
  %ril.0.i.i = load i32* %ril.0.in.i.i, align 4
  %121 = ashr i32 %ril.0.i.i, 2
  %122 = getelementptr inbounds [16 x i32]* @qq4_code4_table, i32 0, i32 %121
  %123 = load i32* %122, align 4, !tbaa !1
  %124 = mul nsw i32 %123, %detl.0
  %125 = ashr i32 %124, 15
  %126 = mul nsw i32 %nbl.0, 127
  %127 = ashr i32 %126, 7
  %128 = getelementptr inbounds [16 x i32]* @wl_code_table, i32 0, i32 %121
  %129 = load i32* %128, align 4, !tbaa !1
  %130 = add nsw i32 %127, %129
  %131 = icmp slt i32 %130, 0
  %..i12.i3 = select i1 %131, i32 0, i32 %130
  %132 = icmp sgt i32 %..i12.i3, 18432
  %...i13.i4 = select i1 %132, i32 18432, i32 %..i12.i3
  %133 = lshr i32 %...i13.i4, 6
  %134 = and i32 %133, 31
  %135 = ashr i32 %...i13.i4, 11
  %136 = getelementptr inbounds [32 x i32]* @ilb_table, i32 0, i32 %134
  %137 = load i32* %136, align 4, !tbaa !1
  %138 = sub i32 9, %135
  %139 = ashr i32 %137, %138
  %140 = shl i32 %139, 3
  %141 = add nsw i32 %104, %125
  tail call fastcc void @upzero(i32 %125, i32* getelementptr inbounds ([6 x i32]* @delay_dltx, i32 0, i32 0), i32* getelementptr inbounds ([6 x i32]* @delay_bpl, i32 0, i32 0)) #2
  %142 = shl nsw i32 %al1.0, 2
  %143 = mul nsw i32 %plt1.0, %141
  %144 = icmp sgt i32 %143, -1
  %145 = sub nsw i32 0, %142
  %.1.i8.i5 = select i1 %144, i32 %145, i32 %142
  %146 = ashr i32 %.1.i8.i5, 7
  %147 = mul nsw i32 %plt2.0, %141
  %148 = ashr i32 %147, 31
  %149 = and i32 %148, -256
  %150 = or i32 %149, 128
  %151 = mul nsw i32 %al2.0, 127
  %152 = ashr i32 %151, 7
  %wd4.0.i9.i6 = add nsw i32 %146, %152
  %153 = add nsw i32 %wd4.0.i9.i6, %150
  %154 = icmp sgt i32 %153, 12288
  %..i10.i7 = select i1 %154, i32 12288, i32 %153
  %155 = icmp slt i32 %..i10.i7, -12288
  %...i11.i8 = select i1 %155, i32 -12288, i32 %..i10.i7
  %156 = mul nsw i32 %al1.0, 255
  %157 = ashr i32 %156, 8
  %158 = ashr i32 %143, 31
  %159 = and i32 %158, -384
  %160 = add nsw i32 %157, 192
  %apl1.0.i5.i9 = add nsw i32 %160, %159
  %161 = sub nsw i32 15360, %...i11.i8
  %162 = icmp sgt i32 %apl1.0.i5.i9, %161
  %.apl1.0.i6.i10 = select i1 %162, i32 %161, i32 %apl1.0.i5.i9
  %163 = sub nsw i32 0, %161
  %164 = icmp slt i32 %.apl1.0.i6.i10, %163
  %apl1.2.i7.i11 = select i1 %164, i32 %163, i32 %.apl1.0.i6.i10
  %165 = add nsw i32 %125, %108
  %166 = load i32* getelementptr inbounds ([6 x i32]* @delay_bph, i32 0, i32 0), align 4, !tbaa !1
  %167 = load i32* getelementptr inbounds ([6 x i32]* @delay_dhx, i32 0, i32 0), align 4, !tbaa !1
  %168 = mul nsw i32 %167, %166
  %169 = load i32* getelementptr inbounds ([6 x i32]* @delay_bph, i32 0, i32 1), align 4, !tbaa !1
  %170 = load i32* getelementptr inbounds ([6 x i32]* @delay_dhx, i32 0, i32 1), align 4, !tbaa !1
  %171 = mul nsw i32 %170, %169
  %172 = add nsw i32 %171, %168
  %173 = load i32* getelementptr inbounds ([6 x i32]* @delay_bph, i32 0, i32 2), align 4, !tbaa !1
  %174 = load i32* getelementptr inbounds ([6 x i32]* @delay_dhx, i32 0, i32 2), align 4, !tbaa !1
  %175 = mul nsw i32 %174, %173
  %176 = add nsw i32 %172, %175
  %177 = load i32* getelementptr inbounds ([6 x i32]* @delay_bph, i32 0, i32 3), align 4, !tbaa !1
  %178 = load i32* getelementptr inbounds ([6 x i32]* @delay_dhx, i32 0, i32 3), align 4, !tbaa !1
  %179 = mul nsw i32 %178, %177
  %180 = add nsw i32 %176, %179
  %181 = load i32* getelementptr inbounds ([6 x i32]* @delay_bph, i32 0, i32 4), align 4, !tbaa !1
  %182 = load i32* getelementptr inbounds ([6 x i32]* @delay_dhx, i32 0, i32 4), align 4, !tbaa !1
  %183 = mul nsw i32 %182, %181
  %184 = add nsw i32 %180, %183
  %185 = load i32* getelementptr inbounds ([6 x i32]* @delay_bph, i32 0, i32 5), align 4, !tbaa !1
  %186 = load i32* getelementptr inbounds ([6 x i32]* @delay_dhx, i32 0, i32 5), align 4, !tbaa !1
  %187 = mul nsw i32 %186, %185
  %188 = add nsw i32 %184, %187
  %189 = ashr i32 %188, 14
  %190 = mul i32 %ah1.0, %rh1.0
  %191 = mul i32 %ah2.0, %rh2.0
  %tmp.i3.i12 = add i32 %191, %190
  %tmp1.i4.i13 = shl i32 %tmp.i3.i12, 1
  %192 = ashr i32 %tmp1.i4.i13, 15
  %193 = add nsw i32 %192, %189
  %194 = sub nsw i32 %80, %193
  %195 = ashr i32 %194, 31
  %196 = and i32 %195, -2
  %197 = add nsw i32 %196, 3
  %198 = mul nsw i32 %deth.0, 564
  %199 = ashr i32 %198, 12
  %200 = icmp sgt i32 %194, -1
  %201 = sub nsw i32 0, %194
  %m.0.i.i = select i1 %200, i32 %194, i32 %201
  %202 = icmp sgt i32 %m.0.i.i, %199
  %203 = add nsw i32 %196, 2
  %. = select i1 %202, i32 %203, i32 %197
  %204 = getelementptr inbounds [4 x i32]* @qq2_code2_table, i32 0, i32 %.
  %205 = load i32* %204, align 4, !tbaa !1
  %206 = mul nsw i32 %205, %deth.0
  %207 = ashr i32 %206, 15
  %208 = mul nsw i32 %nbh.0, 127
  %209 = ashr i32 %208, 7
  %210 = getelementptr inbounds [4 x i32]* @wh_code_table, i32 0, i32 %.
  %211 = load i32* %210, align 4, !tbaa !1
  %212 = add nsw i32 %209, %211
  %213 = icmp slt i32 %212, 0
  %..i1.i14 = select i1 %213, i32 0, i32 %212
  %214 = icmp sgt i32 %..i1.i14, 22528
  %...i2.i15 = select i1 %214, i32 22528, i32 %..i1.i14
  %215 = lshr i32 %...i2.i15, 6
  %216 = and i32 %215, 31
  %217 = ashr i32 %...i2.i15, 11
  %218 = getelementptr inbounds [32 x i32]* @ilb_table, i32 0, i32 %216
  %219 = load i32* %218, align 4, !tbaa !1
  %220 = sub i32 11, %217
  %221 = ashr i32 %219, %220
  %222 = shl i32 %221, 3
  %223 = add nsw i32 %189, %207
  tail call fastcc void @upzero(i32 %207, i32* getelementptr inbounds ([6 x i32]* @delay_dhx, i32 0, i32 0), i32* getelementptr inbounds ([6 x i32]* @delay_bph, i32 0, i32 0)) #2
  %224 = shl nsw i32 %ah1.0, 2
  %225 = mul nsw i32 %ph1.0, %223
  %226 = icmp sgt i32 %225, -1
  %227 = sub nsw i32 0, %224
  %.1.i.i16 = select i1 %226, i32 %227, i32 %224
  %228 = ashr i32 %.1.i.i16, 7
  %229 = mul nsw i32 %ph2.0, %223
  %230 = ashr i32 %229, 31
  %231 = and i32 %230, -256
  %232 = or i32 %231, 128
  %233 = mul nsw i32 %ah2.0, 127
  %234 = ashr i32 %233, 7
  %wd4.0.i.i17 = add nsw i32 %228, %234
  %235 = add nsw i32 %wd4.0.i.i17, %232
  %236 = icmp sgt i32 %235, 12288
  %..i.i18 = select i1 %236, i32 12288, i32 %235
  %237 = icmp slt i32 %..i.i18, -12288
  %...i.i19 = select i1 %237, i32 -12288, i32 %..i.i18
  %238 = mul nsw i32 %ah1.0, 255
  %239 = ashr i32 %238, 8
  %240 = ashr i32 %225, 31
  %241 = and i32 %240, -384
  %242 = add nsw i32 %239, 192
  %apl1.0.i.i20 = add nsw i32 %242, %241
  %243 = sub nsw i32 15360, %...i.i19
  %244 = icmp sgt i32 %apl1.0.i.i20, %243
  %.apl1.0.i.i21 = select i1 %244, i32 %243, i32 %apl1.0.i.i20
  %245 = sub nsw i32 0, %243
  %246 = icmp slt i32 %.apl1.0.i.i21, %245
  %apl1.2.i.i22 = select i1 %246, i32 %245, i32 %.apl1.0.i.i21
  %247 = add nsw i32 %207, %193
  %248 = shl i32 %., 6
  %249 = or i32 %248, %ril.0.i.i
  %250 = sdiv i32 %i.02.i, 2
  %251 = getelementptr inbounds [100 x i32]* @compressed, i32 0, i32 %250
  store i32 %249, i32* %251, align 4, !tbaa !1
  %252 = add nsw i32 %i.02.i, 2
  %253 = icmp slt i32 %252, 100
  br i1 %253, label %encode.exit._crit_edge, label %.preheader.i.preheader

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
  br label %1

.preheader.i.preheader:                           ; preds = %quantl.exit.i
  %ril.0.i.i.lcssa = phi i32 [ %ril.0.i.i, %quantl.exit.i ]
  %254 = getelementptr inbounds [64 x i32]* @qq6_code6_table, i32 0, i32 %ril.0.i.i.lcssa
  %255 = load i32* %254, align 4, !tbaa !1
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
  %accumd.0.0 = phi i32 [ %410, %.preheader.i ], [ 0, %.preheader.i.preheader ]
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
  %accumc.0.0 = phi i32 [ %409, %.preheader.i ], [ 0, %.preheader.i.preheader ]
  %dec_ph2.0 = phi i32 [ %dec_ph1.0, %.preheader.i ], [ 0, %.preheader.i.preheader ]
  %dec_ph1.0 = phi i32 [ %384, %.preheader.i ], [ 0, %.preheader.i.preheader ]
  %dec_nbh.0 = phi i32 [ %...i2.i, %.preheader.i ], [ 0, %.preheader.i.preheader ]
  %dec_deth.0 = phi i32 [ %383, %.preheader.i ], [ 8, %.preheader.i.preheader ]
  %dec_ah2.0 = phi i32 [ %...i.i, %.preheader.i ], [ 0, %.preheader.i.preheader ]
  %dec_rh2.0 = phi i32 [ %dec_rh1.0, %.preheader.i ], [ 0, %.preheader.i.preheader ]
  %dec_ah1.0 = phi i32 [ %apl1.2.i.i, %.preheader.i ], [ 0, %.preheader.i.preheader ]
  %dec_rh1.0 = phi i32 [ %408, %.preheader.i ], [ 0, %.preheader.i.preheader ]
  %dec_plt2.0 = phi i32 [ %dec_plt1.0, %.preheader.i ], [ 0, %.preheader.i.preheader ]
  %dec_plt1.0 = phi i32 [ %312, %.preheader.i ], [ 0, %.preheader.i.preheader ]
  %dec_nbl.0 = phi i32 [ %...i13.i, %.preheader.i ], [ 0, %.preheader.i.preheader ]
  %dec_detl.0 = phi i32 [ %311, %.preheader.i ], [ 32, %.preheader.i.preheader ]
  %dec_al2.0 = phi i32 [ %...i11.i, %.preheader.i ], [ 0, %.preheader.i.preheader ]
  %dec_rlt2.0 = phi i32 [ %dec_rlt1.0, %.preheader.i ], [ 0, %.preheader.i.preheader ]
  %dec_al1.0 = phi i32 [ %apl1.2.i7.i, %.preheader.i ], [ 0, %.preheader.i.preheader ]
  %dec_rlt1.0 = phi i32 [ %336, %.preheader.i ], [ 0, %.preheader.i.preheader ]
  %i.11.i = phi i32 [ %462, %.preheader.i ], [ 0, %.preheader.i.preheader ]
  %256 = sdiv i32 %i.11.i, 2
  %257 = getelementptr inbounds [100 x i32]* @compressed, i32 0, i32 %256
  %258 = load i32* %257, align 4, !tbaa !1
  %259 = and i32 %258, 63
  %260 = ashr i32 %258, 6
  %261 = load i32* getelementptr inbounds ([6 x i32]* @dec_del_bpl, i32 0, i32 0), align 4, !tbaa !1
  %262 = load i32* getelementptr inbounds ([6 x i32]* @dec_del_dltx, i32 0, i32 0), align 4, !tbaa !1
  %263 = mul nsw i32 %262, %261
  %264 = load i32* getelementptr inbounds ([6 x i32]* @dec_del_bpl, i32 0, i32 1), align 4, !tbaa !1
  %265 = load i32* getelementptr inbounds ([6 x i32]* @dec_del_dltx, i32 0, i32 1), align 4, !tbaa !1
  %266 = mul nsw i32 %265, %264
  %267 = add nsw i32 %266, %263
  %268 = load i32* getelementptr inbounds ([6 x i32]* @dec_del_bpl, i32 0, i32 2), align 4, !tbaa !1
  %269 = load i32* getelementptr inbounds ([6 x i32]* @dec_del_dltx, i32 0, i32 2), align 4, !tbaa !1
  %270 = mul nsw i32 %269, %268
  %271 = add nsw i32 %267, %270
  %272 = load i32* getelementptr inbounds ([6 x i32]* @dec_del_bpl, i32 0, i32 3), align 4, !tbaa !1
  %273 = load i32* getelementptr inbounds ([6 x i32]* @dec_del_dltx, i32 0, i32 3), align 4, !tbaa !1
  %274 = mul nsw i32 %273, %272
  %275 = add nsw i32 %271, %274
  %276 = load i32* getelementptr inbounds ([6 x i32]* @dec_del_bpl, i32 0, i32 4), align 4, !tbaa !1
  %277 = load i32* getelementptr inbounds ([6 x i32]* @dec_del_dltx, i32 0, i32 4), align 4, !tbaa !1
  %278 = mul nsw i32 %277, %276
  %279 = add nsw i32 %275, %278
  %280 = load i32* getelementptr inbounds ([6 x i32]* @dec_del_bpl, i32 0, i32 5), align 4, !tbaa !1
  %281 = load i32* getelementptr inbounds ([6 x i32]* @dec_del_dltx, i32 0, i32 5), align 4, !tbaa !1
  %282 = mul nsw i32 %281, %280
  %283 = add nsw i32 %279, %282
  %284 = ashr i32 %283, 14
  %285 = mul i32 %dec_al1.0, %dec_rlt1.0
  %286 = mul i32 %dec_al2.0, %dec_rlt2.0
  %tmp.i.i = add i32 %286, %285
  %tmp1.i.i = shl i32 %tmp.i.i, 1
  %287 = ashr i32 %tmp1.i.i, 15
  %288 = add nsw i32 %287, %284
  %289 = lshr i32 %259, 2
  %290 = getelementptr inbounds [16 x i32]* @qq4_code4_table, i32 0, i32 %289
  %291 = load i32* %290, align 4, !tbaa !1
  %292 = mul nsw i32 %291, %dec_detl.0
  %293 = ashr i32 %292, 15
  %294 = mul nsw i32 %255, %dec_detl.0
  %295 = ashr i32 %294, 15
  %296 = add nsw i32 %295, %288
  %297 = mul nsw i32 %dec_nbl.0, 127
  %298 = ashr i32 %297, 7
  %299 = getelementptr inbounds [16 x i32]* @wl_code_table, i32 0, i32 %289
  %300 = load i32* %299, align 4, !tbaa !1
  %301 = add nsw i32 %298, %300
  %302 = icmp slt i32 %301, 0
  %..i12.i = select i1 %302, i32 0, i32 %301
  %303 = icmp sgt i32 %..i12.i, 18432
  %...i13.i = select i1 %303, i32 18432, i32 %..i12.i
  %304 = lshr i32 %...i13.i, 6
  %305 = and i32 %304, 31
  %306 = ashr i32 %...i13.i, 11
  %307 = getelementptr inbounds [32 x i32]* @ilb_table, i32 0, i32 %305
  %308 = load i32* %307, align 4, !tbaa !1
  %309 = sub i32 9, %306
  %310 = ashr i32 %308, %309
  %311 = shl i32 %310, 3
  %312 = add nsw i32 %284, %293
  tail call fastcc void @upzero(i32 %293, i32* getelementptr inbounds ([6 x i32]* @dec_del_dltx, i32 0, i32 0), i32* getelementptr inbounds ([6 x i32]* @dec_del_bpl, i32 0, i32 0)) #2
  %313 = shl nsw i32 %dec_al1.0, 2
  %314 = mul nsw i32 %dec_plt1.0, %312
  %315 = icmp sgt i32 %314, -1
  %316 = sub nsw i32 0, %313
  %.1.i8.i = select i1 %315, i32 %316, i32 %313
  %317 = ashr i32 %.1.i8.i, 7
  %318 = mul nsw i32 %dec_plt2.0, %312
  %319 = ashr i32 %318, 31
  %320 = and i32 %319, -256
  %321 = or i32 %320, 128
  %322 = mul nsw i32 %dec_al2.0, 127
  %323 = ashr i32 %322, 7
  %wd4.0.i9.i = add nsw i32 %317, %323
  %324 = add nsw i32 %wd4.0.i9.i, %321
  %325 = icmp sgt i32 %324, 12288
  %..i10.i = select i1 %325, i32 12288, i32 %324
  %326 = icmp slt i32 %..i10.i, -12288
  %...i11.i = select i1 %326, i32 -12288, i32 %..i10.i
  %327 = mul nsw i32 %dec_al1.0, 255
  %328 = ashr i32 %327, 8
  %329 = ashr i32 %314, 31
  %330 = and i32 %329, -384
  %331 = add nsw i32 %328, 192
  %apl1.0.i5.i = add nsw i32 %331, %330
  %332 = sub nsw i32 15360, %...i11.i
  %333 = icmp sgt i32 %apl1.0.i5.i, %332
  %.apl1.0.i6.i = select i1 %333, i32 %332, i32 %apl1.0.i5.i
  %334 = sub nsw i32 0, %332
  %335 = icmp slt i32 %.apl1.0.i6.i, %334
  %apl1.2.i7.i = select i1 %335, i32 %334, i32 %.apl1.0.i6.i
  %336 = add nsw i32 %293, %288
  %337 = load i32* getelementptr inbounds ([6 x i32]* @dec_del_bph, i32 0, i32 0), align 4, !tbaa !1
  %338 = load i32* getelementptr inbounds ([6 x i32]* @dec_del_dhx, i32 0, i32 0), align 4, !tbaa !1
  %339 = mul nsw i32 %338, %337
  %340 = load i32* getelementptr inbounds ([6 x i32]* @dec_del_bph, i32 0, i32 1), align 4, !tbaa !1
  %341 = load i32* getelementptr inbounds ([6 x i32]* @dec_del_dhx, i32 0, i32 1), align 4, !tbaa !1
  %342 = mul nsw i32 %341, %340
  %343 = add nsw i32 %342, %339
  %344 = load i32* getelementptr inbounds ([6 x i32]* @dec_del_bph, i32 0, i32 2), align 4, !tbaa !1
  %345 = load i32* getelementptr inbounds ([6 x i32]* @dec_del_dhx, i32 0, i32 2), align 4, !tbaa !1
  %346 = mul nsw i32 %345, %344
  %347 = add nsw i32 %343, %346
  %348 = load i32* getelementptr inbounds ([6 x i32]* @dec_del_bph, i32 0, i32 3), align 4, !tbaa !1
  %349 = load i32* getelementptr inbounds ([6 x i32]* @dec_del_dhx, i32 0, i32 3), align 4, !tbaa !1
  %350 = mul nsw i32 %349, %348
  %351 = add nsw i32 %347, %350
  %352 = load i32* getelementptr inbounds ([6 x i32]* @dec_del_bph, i32 0, i32 4), align 4, !tbaa !1
  %353 = load i32* getelementptr inbounds ([6 x i32]* @dec_del_dhx, i32 0, i32 4), align 4, !tbaa !1
  %354 = mul nsw i32 %353, %352
  %355 = add nsw i32 %351, %354
  %356 = load i32* getelementptr inbounds ([6 x i32]* @dec_del_bph, i32 0, i32 5), align 4, !tbaa !1
  %357 = load i32* getelementptr inbounds ([6 x i32]* @dec_del_dhx, i32 0, i32 5), align 4, !tbaa !1
  %358 = mul nsw i32 %357, %356
  %359 = add nsw i32 %355, %358
  %360 = ashr i32 %359, 14
  %361 = mul i32 %dec_ah1.0, %dec_rh1.0
  %362 = mul i32 %dec_ah2.0, %dec_rh2.0
  %tmp.i3.i = add i32 %362, %361
  %tmp1.i4.i = shl i32 %tmp.i3.i, 1
  %363 = ashr i32 %tmp1.i4.i, 15
  %364 = add nsw i32 %363, %360
  %365 = getelementptr inbounds [4 x i32]* @qq2_code2_table, i32 0, i32 %260
  %366 = load i32* %365, align 4, !tbaa !1
  %367 = mul nsw i32 %366, %dec_deth.0
  %368 = ashr i32 %367, 15
  %369 = mul nsw i32 %dec_nbh.0, 127
  %370 = ashr i32 %369, 7
  %371 = getelementptr inbounds [4 x i32]* @wh_code_table, i32 0, i32 %260
  %372 = load i32* %371, align 4, !tbaa !1
  %373 = add nsw i32 %370, %372
  %374 = icmp slt i32 %373, 0
  %..i1.i = select i1 %374, i32 0, i32 %373
  %375 = icmp sgt i32 %..i1.i, 22528
  %...i2.i = select i1 %375, i32 22528, i32 %..i1.i
  %376 = lshr i32 %...i2.i, 6
  %377 = and i32 %376, 31
  %378 = ashr i32 %...i2.i, 11
  %379 = getelementptr inbounds [32 x i32]* @ilb_table, i32 0, i32 %377
  %380 = load i32* %379, align 4, !tbaa !1
  %381 = sub i32 11, %378
  %382 = ashr i32 %380, %381
  %383 = shl i32 %382, 3
  %384 = add nsw i32 %360, %368
  tail call fastcc void @upzero(i32 %368, i32* getelementptr inbounds ([6 x i32]* @dec_del_dhx, i32 0, i32 0), i32* getelementptr inbounds ([6 x i32]* @dec_del_bph, i32 0, i32 0)) #2
  %385 = shl nsw i32 %dec_ah1.0, 2
  %386 = mul nsw i32 %dec_ph1.0, %384
  %387 = icmp sgt i32 %386, -1
  %388 = sub nsw i32 0, %385
  %.1.i.i = select i1 %387, i32 %388, i32 %385
  %389 = ashr i32 %.1.i.i, 7
  %390 = mul nsw i32 %dec_ph2.0, %384
  %391 = ashr i32 %390, 31
  %392 = and i32 %391, -256
  %393 = or i32 %392, 128
  %394 = mul nsw i32 %dec_ah2.0, 127
  %395 = ashr i32 %394, 7
  %wd4.0.i.i = add nsw i32 %389, %395
  %396 = add nsw i32 %wd4.0.i.i, %393
  %397 = icmp sgt i32 %396, 12288
  %..i.i = select i1 %397, i32 12288, i32 %396
  %398 = icmp slt i32 %..i.i, -12288
  %...i.i = select i1 %398, i32 -12288, i32 %..i.i
  %399 = mul nsw i32 %dec_ah1.0, 255
  %400 = ashr i32 %399, 8
  %401 = ashr i32 %386, 31
  %402 = and i32 %401, -384
  %403 = add nsw i32 %400, 192
  %apl1.0.i.i = add nsw i32 %403, %402
  %404 = sub nsw i32 15360, %...i.i
  %405 = icmp sgt i32 %apl1.0.i.i, %404
  %.apl1.0.i.i = select i1 %405, i32 %404, i32 %apl1.0.i.i
  %406 = sub nsw i32 0, %404
  %407 = icmp slt i32 %.apl1.0.i.i, %406
  %apl1.2.i.i = select i1 %407, i32 %406, i32 %.apl1.0.i.i
  %408 = add nsw i32 %368, %364
  %409 = sub nsw i32 %296, %408
  %410 = add nsw i32 %408, %296
  %411 = mul nsw i32 %409, 12
  %412 = mul nsw i32 %410, -44
  %413 = mul nsw i32 %accumc.0.0, -44
  %414 = add nsw i32 %413, %411
  %415 = mul nsw i32 %accumd.0.0, 212
  %416 = add nsw i32 %415, %412
  %417 = mul nsw i32 %accumc.1.0, 48
  %418 = add nsw i32 %417, %414
  %419 = mul nsw i32 %accumd.1.0, -624
  %420 = add nsw i32 %419, %416
  %421 = shl nsw i32 %accumc.2.0, 7
  %422 = add nsw i32 %421, %418
  %423 = mul nsw i32 %accumd.2.0, 1448
  %424 = add nsw i32 %423, %420
  %425 = mul nsw i32 %accumc.3.0, -840
  %426 = add nsw i32 %425, %422
  %427 = mul nsw i32 %accumd.3.0, -3220
  %428 = add nsw i32 %427, %424
  %429 = mul nsw i32 %accumc.4.0, 3804
  %430 = add nsw i32 %429, %426
  %431 = mul nsw i32 %accumd.4.0, 15504
  %432 = add nsw i32 %431, %428
  %433 = mul nsw i32 %accumc.5.0, 15504
  %434 = add nsw i32 %433, %430
  %435 = mul nsw i32 %accumd.5.0, 3804
  %436 = add nsw i32 %435, %432
  %437 = mul nsw i32 %accumc.6.0, -3220
  %438 = add nsw i32 %437, %434
  %439 = mul nsw i32 %accumd.6.0, -840
  %440 = add nsw i32 %439, %436
  %441 = mul nsw i32 %accumc.7.0, 1448
  %442 = add nsw i32 %441, %438
  %443 = shl nsw i32 %accumd.7.0, 7
  %444 = add nsw i32 %443, %440
  %445 = mul nsw i32 %accumc.8.0, -624
  %446 = add nsw i32 %445, %442
  %447 = mul nsw i32 %accumd.8.0, 48
  %448 = add nsw i32 %447, %444
  %449 = mul nsw i32 %accumc.9.0, 212
  %450 = add nsw i32 %449, %446
  %451 = mul nsw i32 %accumd.9.0, -44
  %452 = add nsw i32 %451, %448
  %453 = mul nsw i32 %accumc.10.0, -44
  %454 = add nsw i32 %453, %450
  %455 = mul nsw i32 %accumd.10.0, 12
  %456 = add nsw i32 %455, %452
  %457 = ashr i32 %454, 14
  %458 = ashr i32 %456, 14
  %459 = getelementptr inbounds [100 x i32]* @result, i32 0, i32 %i.11.i
  store i32 %457, i32* %459, align 4, !tbaa !1
  %460 = or i32 %i.11.i, 1
  %461 = getelementptr inbounds [100 x i32]* @result, i32 0, i32 %460
  store i32 %458, i32* %461, align 4, !tbaa !1
  %462 = add nsw i32 %i.11.i, 2
  %463 = icmp slt i32 %462, 100
  br i1 %463, label %.preheader.i, label %adpcm_main.exit

adpcm_main.exit:                                  ; preds = %adpcm_main.exit, %.preheader.i
  %main_result.04 = phi i32 [ %.main_result.0, %adpcm_main.exit ], [ 0, %.preheader.i ]
  %i.03 = phi i32 [ %470, %adpcm_main.exit ], [ 0, %.preheader.i ]
  %464 = getelementptr inbounds [100 x i32]* @compressed, i32 0, i32 %i.03
  %465 = load i32* %464, align 4, !tbaa !1
  %466 = getelementptr inbounds [100 x i32]* @test_compressed, i32 0, i32 %i.03
  %467 = load i32* %466, align 4, !tbaa !1
  %468 = icmp eq i32 %465, %467
  %469 = zext i1 %468 to i32
  %.main_result.0 = add nsw i32 %469, %main_result.04
  %470 = add nsw i32 %i.03, 1
  %exitcond5 = icmp eq i32 %470, 50
  br i1 %exitcond5, label %.preheader, label %adpcm_main.exit

.preheader:                                       ; preds = %.preheader, %adpcm_main.exit
  %main_result.22 = phi i32 [ %.main_result.2, %.preheader ], [ %.main_result.0, %adpcm_main.exit ]
  %i.11 = phi i32 [ %477, %.preheader ], [ 0, %adpcm_main.exit ]
  %471 = getelementptr inbounds [100 x i32]* @result, i32 0, i32 %i.11
  %472 = load i32* %471, align 4, !tbaa !1
  %473 = getelementptr inbounds [100 x i32]* @test_result, i32 0, i32 %i.11
  %474 = load i32* %473, align 4, !tbaa !1
  %475 = icmp eq i32 %472, %474
  %476 = zext i1 %475 to i32
  %.main_result.2 = add nsw i32 %476, %main_result.22
  %477 = add nsw i32 %i.11, 1
  %exitcond = icmp eq i32 %477, 100
  br i1 %exitcond, label %478, label %.preheader

; <label>:478                                     ; preds = %.preheader
  %.main_result.2.lcssa = phi i32 [ %.main_result.2, %.preheader ]
  %479 = tail call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([12 x i8]* @.str, i32 0, i32 0), i32 %.main_result.2.lcssa) #2
  %480 = icmp eq i32 %.main_result.2.lcssa, 150
  br i1 %480, label %481, label %483

; <label>:481                                     ; preds = %478
  %482 = tail call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([14 x i8]* @.str1, i32 0, i32 0)) #2
  br label %485

; <label>:483                                     ; preds = %478
  %484 = tail call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([14 x i8]* @.str2, i32 0, i32 0)) #2
  br label %485

; <label>:485                                     ; preds = %483, %481
  ret i32 %.main_result.2.lcssa
}

; Function Attrs: nounwind
declare i32 @printf(i8* nocapture readonly, ...) #0

; Function Attrs: nounwind
define internal void @legup_memset_4_i64(i8* %m, i8 zeroext %c1, i64 %n) #1 {
  %1 = alloca i8*, align 4
  %2 = alloca i8, align 1
  %3 = alloca i64, align 8
  %c = alloca i32, align 4
  %s = alloca i32*, align 4
  store i8* %m, i8** %1, align 4
  store i8 %c1, i8* %2, align 1
  store i64 %n, i64* %3, align 8
  %4 = load i8* %2, align 1
  %5 = zext i8 %4 to i32
  store i32 %5, i32* %c, align 4
  %6 = load i32* %c, align 4
  %7 = shl i32 %6, 8
  %8 = load i32* %c, align 4
  %9 = or i32 %8, %7
  store i32 %9, i32* %c, align 4
  %10 = load i32* %c, align 4
  %11 = shl i32 %10, 16
  %12 = load i32* %c, align 4
  %13 = or i32 %12, %11
  store i32 %13, i32* %c, align 4
  %14 = load i8** %1, align 4
  %15 = bitcast i8* %14 to i32*
  store i32* %15, i32** %s, align 4
  %16 = load i64* %3, align 8
  %17 = lshr i64 %16, 2
  store i64 %17, i64* %3, align 8
  br label %18

; <label>:18                                      ; preds = %22, %0
  %19 = load i64* %3, align 8
  %20 = add i64 %19, -1
  store i64 %20, i64* %3, align 8
  %21 = icmp ne i64 %19, 0
  br i1 %21, label %22, label %26

; <label>:22                                      ; preds = %18
  %23 = load i32* %c, align 4
  %24 = load i32** %s, align 4
  %25 = getelementptr inbounds i32* %24, i32 1
  store i32* %25, i32** %s, align 4
  store i32 %23, i32* %24, align 4
  br label %18

; <label>:26                                      ; preds = %18
  ret void
}

attributes #0 = { nounwind "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nobuiltin nounwind }

!llvm.ident = !{!0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0}

!0 = metadata !{metadata !"clang version 3.5.0 (tags/RELEASE_350/final)"}
!1 = metadata !{metadata !2, metadata !2, i64 0}
!2 = metadata !{metadata !"int", metadata !3, i64 0}
!3 = metadata !{metadata !"omnipotent char", metadata !4, i64 0}
!4 = metadata !{metadata !"Simple C/C++ TBAA"}
