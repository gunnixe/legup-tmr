; ModuleID = 'adpcm.prelto.linked.bc'
target datalayout = "e-m:e-p:32:32-f64:32:64-f80:32-n8:16:32-S128"
target triple = "i386-unknown-linux-gnu"

@h = constant [24 x i32] [i32 12, i32 -44, i32 -44, i32 212, i32 48, i32 -624, i32 128, i32 1448, i32 -840, i32 -3220, i32 3804, i32 15504, i32 15504, i32 3804, i32 -3220, i32 -840, i32 1448, i32 128, i32 -624, i32 48, i32 212, i32 -44, i32 -44, i32 12], align 4
@qq4_code4_table = constant [16 x i32] [i32 0, i32 -20456, i32 -12896, i32 -8968, i32 -6288, i32 -4240, i32 -2584, i32 -1200, i32 20456, i32 12896, i32 8968, i32 6288, i32 4240, i32 2584, i32 1200, i32 0], align 4
@qq6_code6_table = constant [64 x i32] [i32 -136, i32 -136, i32 -136, i32 -136, i32 -24808, i32 -21904, i32 -19008, i32 -16704, i32 -14984, i32 -13512, i32 -12280, i32 -11192, i32 -10232, i32 -9360, i32 -8576, i32 -7856, i32 -7192, i32 -6576, i32 -6000, i32 -5456, i32 -4944, i32 -4464, i32 -4008, i32 -3576, i32 -3168, i32 -2776, i32 -2400, i32 -2032, i32 -1688, i32 -1360, i32 -1040, i32 -728, i32 24808, i32 21904, i32 19008, i32 16704, i32 14984, i32 13512, i32 12280, i32 11192, i32 10232, i32 9360, i32 8576, i32 7856, i32 7192, i32 6576, i32 6000, i32 5456, i32 4944, i32 4464, i32 4008, i32 3576, i32 3168, i32 2776, i32 2400, i32 2032, i32 1688, i32 1360, i32 1040, i32 728, i32 432, i32 136, i32 -432, i32 -136], align 4
@wl_code_table = constant [16 x i32] [i32 -60, i32 3042, i32 1198, i32 538, i32 334, i32 172, i32 58, i32 -30, i32 3042, i32 1198, i32 538, i32 334, i32 172, i32 58, i32 -30, i32 -60], align 4
@ilb_table = constant [32 x i32] [i32 2048, i32 2093, i32 2139, i32 2186, i32 2233, i32 2282, i32 2332, i32 2383, i32 2435, i32 2489, i32 2543, i32 2599, i32 2656, i32 2714, i32 2774, i32 2834, i32 2896, i32 2960, i32 3025, i32 3091, i32 3158, i32 3228, i32 3298, i32 3371, i32 3444, i32 3520, i32 3597, i32 3676, i32 3756, i32 3838, i32 3922, i32 4008], align 4
@decis_levl = constant [30 x i32] [i32 280, i32 576, i32 880, i32 1200, i32 1520, i32 1864, i32 2208, i32 2584, i32 2960, i32 3376, i32 3784, i32 4240, i32 4696, i32 5200, i32 5712, i32 6288, i32 6864, i32 7520, i32 8184, i32 8968, i32 9752, i32 10712, i32 11664, i32 12896, i32 14120, i32 15840, i32 17560, i32 20456, i32 23352, i32 32767], align 4
@quant26bt_pos = constant [31 x i32] [i32 61, i32 60, i32 59, i32 58, i32 57, i32 56, i32 55, i32 54, i32 53, i32 52, i32 51, i32 50, i32 49, i32 48, i32 47, i32 46, i32 45, i32 44, i32 43, i32 42, i32 41, i32 40, i32 39, i32 38, i32 37, i32 36, i32 35, i32 34, i32 33, i32 32, i32 32], align 4
@quant26bt_neg = constant [31 x i32] [i32 63, i32 62, i32 31, i32 30, i32 29, i32 28, i32 27, i32 26, i32 25, i32 24, i32 23, i32 22, i32 21, i32 20, i32 19, i32 18, i32 17, i32 16, i32 15, i32 14, i32 13, i32 12, i32 11, i32 10, i32 9, i32 8, i32 7, i32 6, i32 5, i32 4, i32 4], align 4
@qq2_code2_table = constant [4 x i32] [i32 -7408, i32 -1616, i32 7408, i32 1616], align 4
@wh_code_table = constant [4 x i32] [i32 798, i32 -214, i32 798, i32 -214], align 4
@tqmf = common global [24 x i32] zeroinitializer, align 4
@xl = common global i32 0, align 4
@xh = common global i32 0, align 4
@delay_bpl = common global [6 x i32] zeroinitializer, align 4
@delay_dltx = common global [6 x i32] zeroinitializer, align 4
@szl = common global i32 0, align 4
@rlt1 = common global i32 0, align 4
@al1 = common global i32 0, align 4
@rlt2 = common global i32 0, align 4
@al2 = common global i32 0, align 4
@spl = common global i32 0, align 4
@sl = common global i32 0, align 4
@el = common global i32 0, align 4
@detl = common global i32 0, align 4
@il = common global i32 0, align 4
@dlt = common global i32 0, align 4
@nbl = common global i32 0, align 4
@plt = common global i32 0, align 4
@plt1 = common global i32 0, align 4
@plt2 = common global i32 0, align 4
@rlt = common global i32 0, align 4
@delay_bph = common global [6 x i32] zeroinitializer, align 4
@delay_dhx = common global [6 x i32] zeroinitializer, align 4
@szh = common global i32 0, align 4
@rh1 = common global i32 0, align 4
@ah1 = common global i32 0, align 4
@rh2 = common global i32 0, align 4
@ah2 = common global i32 0, align 4
@sph = common global i32 0, align 4
@sh = common global i32 0, align 4
@eh = common global i32 0, align 4
@ih = common global i32 0, align 4
@deth = common global i32 0, align 4
@dh = common global i32 0, align 4
@nbh = common global i32 0, align 4
@ph = common global i32 0, align 4
@ph1 = common global i32 0, align 4
@ph2 = common global i32 0, align 4
@yh = common global i32 0, align 4
@ilr = common global i32 0, align 4
@dec_del_bpl = common global [6 x i32] zeroinitializer, align 4
@dec_del_dltx = common global [6 x i32] zeroinitializer, align 4
@dec_szl = common global i32 0, align 4
@dec_rlt1 = common global i32 0, align 4
@dec_al1 = common global i32 0, align 4
@dec_rlt2 = common global i32 0, align 4
@dec_al2 = common global i32 0, align 4
@dec_spl = common global i32 0, align 4
@dec_sl = common global i32 0, align 4
@dec_detl = common global i32 0, align 4
@dec_dlt = common global i32 0, align 4
@dl = common global i32 0, align 4
@rl = common global i32 0, align 4
@dec_nbl = common global i32 0, align 4
@dec_plt = common global i32 0, align 4
@dec_plt1 = common global i32 0, align 4
@dec_plt2 = common global i32 0, align 4
@dec_rlt = common global i32 0, align 4
@dec_del_bph = common global [6 x i32] zeroinitializer, align 4
@dec_del_dhx = common global [6 x i32] zeroinitializer, align 4
@dec_szh = common global i32 0, align 4
@dec_rh1 = common global i32 0, align 4
@dec_ah1 = common global i32 0, align 4
@dec_rh2 = common global i32 0, align 4
@dec_ah2 = common global i32 0, align 4
@dec_sph = common global i32 0, align 4
@dec_sh = common global i32 0, align 4
@dec_deth = common global i32 0, align 4
@dec_dh = common global i32 0, align 4
@dec_nbh = common global i32 0, align 4
@dec_ph = common global i32 0, align 4
@dec_ph1 = common global i32 0, align 4
@dec_ph2 = common global i32 0, align 4
@rh = common global i32 0, align 4
@xd = common global i32 0, align 4
@xs = common global i32 0, align 4
@accumc = common global [11 x i32] zeroinitializer, align 4
@accumd = common global [11 x i32] zeroinitializer, align 4
@xout1 = common global i32 0, align 4
@xout2 = common global i32 0, align 4
@test_data = constant [100 x i32] [i32 68, i32 68, i32 68, i32 68, i32 68, i32 68, i32 68, i32 68, i32 68, i32 68, i32 68, i32 68, i32 68, i32 68, i32 68, i32 68, i32 68, i32 67, i32 67, i32 67, i32 67, i32 67, i32 67, i32 67, i32 66, i32 66, i32 66, i32 66, i32 66, i32 66, i32 65, i32 65, i32 65, i32 65, i32 65, i32 64, i32 64, i32 64, i32 64, i32 64, i32 64, i32 64, i32 64, i32 63, i32 63, i32 63, i32 63, i32 63, i32 62, i32 62, i32 62, i32 62, i32 62, i32 62, i32 61, i32 61, i32 61, i32 61, i32 61, i32 61, i32 60, i32 60, i32 60, i32 60, i32 60, i32 60, i32 60, i32 60, i32 60, i32 59, i32 59, i32 59, i32 59, i32 59, i32 59, i32 59, i32 59, i32 59, i32 59, i32 59, i32 59, i32 59, i32 59, i32 59, i32 59, i32 59, i32 59, i32 59, i32 59, i32 59, i32 59, i32 59, i32 60, i32 60, i32 60, i32 60, i32 60, i32 60, i32 60, i32 60], align 4
@test_compressed = constant [100 x i32] [i32 253, i32 222, i32 119, i32 186, i32 244, i32 146, i32 32, i32 160, i32 236, i32 237, i32 238, i32 240, i32 241, i32 241, i32 242, i32 243, i32 244, i32 243, i32 244, i32 245, i32 244, i32 244, i32 245, i32 245, i32 245, i32 246, i32 246, i32 247, i32 247, i32 247, i32 247, i32 248, i32 246, i32 247, i32 249, i32 247, i32 248, i32 247, i32 248, i32 247, i32 248, i32 247, i32 248, i32 247, i32 248, i32 248, i32 246, i32 248, i32 247, i32 248, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0], align 4
@test_result = constant [100 x i32] [i32 0, i32 -1, i32 -1, i32 0, i32 0, i32 -1, i32 0, i32 0, i32 -1, i32 -1, i32 0, i32 0, i32 1, i32 1, i32 0, i32 -2, i32 -1, i32 -2, i32 0, i32 -3, i32 1, i32 0, i32 0, i32 -4, i32 1, i32 1, i32 2, i32 11, i32 20, i32 18, i32 20, i32 22, i32 28, i32 27, i32 31, i32 31, i32 34, i32 31, i32 34, i32 34, i32 38, i32 37, i32 42, i32 42, i32 44, i32 41, i32 43, i32 42, i32 47, i32 45, i32 47, i32 44, i32 45, i32 43, i32 46, i32 45, i32 48, i32 46, i32 49, i32 48, i32 51, i32 49, i32 52, i32 52, i32 55, i32 53, i32 56, i32 55, i32 58, i32 57, i32 59, i32 57, i32 60, i32 60, i32 60, i32 54, i32 54, i32 53, i32 60, i32 62, i32 62, i32 54, i32 55, i32 56, i32 59, i32 53, i32 54, i32 56, i32 59, i32 53, i32 56, i32 58, i32 59, i32 53, i32 56, i32 58, i32 60, i32 54, i32 55, i32 57], align 4
@compressed = common global [100 x i32] zeroinitializer, align 4
@result = common global [100 x i32] zeroinitializer, align 4
@.str = private unnamed_addr constant [12 x i8] c"Result: %d\0A\00", align 1
@.str1 = private unnamed_addr constant [14 x i8] c"RESULT: PASS\0A\00", align 1
@.str2 = private unnamed_addr constant [14 x i8] c"RESULT: FAIL\0A\00", align 1

; Function Attrs: nounwind readnone
define i32 @abs(i32 %n) #0 {
  %1 = icmp sgt i32 %n, -1
  %2 = sub nsw i32 0, %n
  %m.0 = select i1 %1, i32 %n, i32 %2
  ret i32 %m.0
}

; Function Attrs: nounwind
define i32 @encode(i32 %xin1, i32 %xin2) #1 {
  %1 = load i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 0), align 4, !tbaa !1
  %2 = mul nsw i32 %1, 12
  %3 = load i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 1), align 4, !tbaa !1
  %4 = mul nsw i32 %3, -44
  %5 = load i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 2), align 4, !tbaa !1
  %6 = mul nsw i32 %5, -44
  %7 = add nsw i32 %6, %2
  %8 = load i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 3), align 4, !tbaa !1
  %9 = mul nsw i32 %8, 212
  %10 = add nsw i32 %9, %4
  %11 = load i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 4), align 4, !tbaa !1
  %12 = mul nsw i32 %11, 48
  %13 = add nsw i32 %12, %7
  %14 = load i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 5), align 4, !tbaa !1
  %15 = mul nsw i32 %14, -624
  %16 = add nsw i32 %15, %10
  %17 = load i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 6), align 4, !tbaa !1
  %18 = shl nsw i32 %17, 7
  %19 = add nsw i32 %18, %13
  %20 = load i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 7), align 4, !tbaa !1
  %21 = mul nsw i32 %20, 1448
  %22 = add nsw i32 %21, %16
  %23 = load i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 8), align 4, !tbaa !1
  %24 = mul nsw i32 %23, -840
  %25 = add nsw i32 %24, %19
  %26 = load i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 9), align 4, !tbaa !1
  %27 = mul nsw i32 %26, -3220
  %28 = add nsw i32 %27, %22
  %29 = load i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 10), align 4, !tbaa !1
  %30 = mul nsw i32 %29, 3804
  %31 = add nsw i32 %30, %25
  %32 = load i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 11), align 4, !tbaa !1
  %33 = mul nsw i32 %32, 15504
  %34 = add nsw i32 %33, %28
  %35 = load i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 12), align 4, !tbaa !1
  %36 = mul nsw i32 %35, 15504
  %37 = add nsw i32 %36, %31
  %38 = load i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 13), align 4, !tbaa !1
  %39 = mul nsw i32 %38, 3804
  %40 = add nsw i32 %39, %34
  %41 = load i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 14), align 4, !tbaa !1
  %42 = mul nsw i32 %41, -3220
  %43 = add nsw i32 %42, %37
  %44 = load i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 15), align 4, !tbaa !1
  %45 = mul nsw i32 %44, -840
  %46 = add nsw i32 %45, %40
  %47 = load i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 16), align 4, !tbaa !1
  %48 = mul nsw i32 %47, 1448
  %49 = add nsw i32 %48, %43
  %50 = load i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 17), align 4, !tbaa !1
  %51 = shl nsw i32 %50, 7
  %52 = add nsw i32 %51, %46
  %53 = load i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 18), align 4, !tbaa !1
  %54 = mul nsw i32 %53, -624
  %55 = add nsw i32 %54, %49
  %56 = load i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 19), align 4, !tbaa !1
  %57 = mul nsw i32 %56, 48
  %58 = add nsw i32 %57, %52
  %59 = load i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 20), align 4, !tbaa !1
  %60 = mul nsw i32 %59, 212
  %61 = add nsw i32 %60, %55
  %62 = load i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 21), align 4, !tbaa !1
  %63 = mul nsw i32 %62, -44
  %64 = add nsw i32 %63, %58
  %65 = load i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 22), align 4, !tbaa !1
  %66 = mul nsw i32 %65, -44
  %67 = add nsw i32 %66, %61
  %68 = load i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 23), align 4, !tbaa !1
  %69 = mul nsw i32 %68, 12
  store i32 %62, i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 23), align 4, !tbaa !1
  store i32 %59, i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 22), align 4, !tbaa !1
  store i32 %56, i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 21), align 4, !tbaa !1
  store i32 %53, i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 20), align 4, !tbaa !1
  store i32 %50, i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 19), align 4, !tbaa !1
  store i32 %47, i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 18), align 4, !tbaa !1
  store i32 %44, i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 17), align 4, !tbaa !1
  store i32 %41, i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 16), align 4, !tbaa !1
  store i32 %38, i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 15), align 4, !tbaa !1
  store i32 %35, i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 14), align 4, !tbaa !1
  store i32 %32, i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 13), align 4, !tbaa !1
  store i32 %29, i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 12), align 4, !tbaa !1
  store i32 %26, i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 11), align 4, !tbaa !1
  store i32 %23, i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 10), align 4, !tbaa !1
  store i32 %20, i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 9), align 4, !tbaa !1
  store i32 %17, i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 8), align 4, !tbaa !1
  store i32 %14, i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 7), align 4, !tbaa !1
  store i32 %11, i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 6), align 4, !tbaa !1
  store i32 %8, i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 5), align 4, !tbaa !1
  store i32 %5, i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 4), align 4, !tbaa !1
  store i32 %3, i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 3), align 4, !tbaa !1
  store i32 %1, i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 2), align 4, !tbaa !1
  %70 = add nsw i32 %69, %64
  store i32 %xin1, i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 1), align 4, !tbaa !1
  store i32 %xin2, i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 0), align 4, !tbaa !1
  %71 = add nsw i32 %70, %67
  %72 = ashr i32 %71, 15
  store i32 %72, i32* @xl, align 4, !tbaa !1
  %73 = sub nsw i32 %67, %70
  %74 = ashr i32 %73, 15
  store i32 %74, i32* @xh, align 4, !tbaa !1
  %75 = load i32* getelementptr inbounds ([6 x i32]* @delay_bpl, i32 0, i32 0), align 4, !tbaa !1
  %76 = load i32* getelementptr inbounds ([6 x i32]* @delay_dltx, i32 0, i32 0), align 4, !tbaa !1
  %77 = mul nsw i32 %76, %75
  %78 = load i32* getelementptr inbounds ([6 x i32]* @delay_bpl, i32 0, i32 1), align 4, !tbaa !1
  %79 = load i32* getelementptr inbounds ([6 x i32]* @delay_dltx, i32 0, i32 1), align 4, !tbaa !1
  %80 = mul nsw i32 %79, %78
  %81 = add nsw i32 %80, %77
  %82 = load i32* getelementptr inbounds ([6 x i32]* @delay_bpl, i32 0, i32 2), align 4, !tbaa !1
  %83 = load i32* getelementptr inbounds ([6 x i32]* @delay_dltx, i32 0, i32 2), align 4, !tbaa !1
  %84 = mul nsw i32 %83, %82
  %85 = add nsw i32 %81, %84
  %86 = load i32* getelementptr inbounds ([6 x i32]* @delay_bpl, i32 0, i32 3), align 4, !tbaa !1
  %87 = load i32* getelementptr inbounds ([6 x i32]* @delay_dltx, i32 0, i32 3), align 4, !tbaa !1
  %88 = mul nsw i32 %87, %86
  %89 = add nsw i32 %85, %88
  %90 = load i32* getelementptr inbounds ([6 x i32]* @delay_bpl, i32 0, i32 4), align 4, !tbaa !1
  %91 = load i32* getelementptr inbounds ([6 x i32]* @delay_dltx, i32 0, i32 4), align 4, !tbaa !1
  %92 = mul nsw i32 %91, %90
  %93 = add nsw i32 %89, %92
  %94 = load i32* getelementptr inbounds ([6 x i32]* @delay_bpl, i32 0, i32 5), align 4, !tbaa !1
  %95 = load i32* getelementptr inbounds ([6 x i32]* @delay_dltx, i32 0, i32 5), align 4, !tbaa !1
  %96 = mul nsw i32 %95, %94
  %97 = add nsw i32 %93, %96
  %98 = ashr i32 %97, 14
  store i32 %98, i32* @szl, align 4, !tbaa !1
  %99 = load i32* @rlt1, align 4, !tbaa !1
  %100 = load i32* @al1, align 4, !tbaa !1
  %101 = load i32* @rlt2, align 4, !tbaa !1
  %102 = load i32* @al2, align 4, !tbaa !1
  %103 = mul i32 %100, %99
  %104 = mul i32 %102, %101
  %tmp.i = add i32 %104, %103
  %tmp1.i = shl i32 %tmp.i, 1
  %105 = ashr i32 %tmp1.i, 15
  store i32 %105, i32* @spl, align 4, !tbaa !1
  %106 = add nsw i32 %105, %98
  store i32 %106, i32* @sl, align 4, !tbaa !1
  %107 = sub nsw i32 %72, %106
  store i32 %107, i32* @el, align 4, !tbaa !1
  %108 = load i32* @detl, align 4, !tbaa !1
  %109 = icmp sgt i32 %107, -1
  %110 = sub nsw i32 0, %107
  %m.0.i.i = select i1 %109, i32 %107, i32 %110
  br label %113

; <label>:111                                     ; preds = %113
  %112 = icmp slt i32 %119, 30
  br i1 %112, label %113, label %quantl.exit

; <label>:113                                     ; preds = %111, %0
  %mil.01.i = phi i32 [ 0, %0 ], [ %119, %111 ]
  %114 = getelementptr inbounds [30 x i32]* @decis_levl, i32 0, i32 %mil.01.i
  %115 = load i32* %114, align 4, !tbaa !1
  %116 = mul nsw i32 %115, %108
  %117 = ashr i32 %116, 15
  %118 = icmp sgt i32 %m.0.i.i, %117
  %119 = add nsw i32 %mil.01.i, 1
  br i1 %118, label %111, label %quantl.exit

quantl.exit:                                      ; preds = %113, %111
  %mil.0.lcssa.i = phi i32 [ %mil.01.i, %113 ], [ %119, %111 ]
  %quant26bt_pos.quant26bt_neg.i = select i1 %109, [31 x i32]* @quant26bt_pos, [31 x i32]* @quant26bt_neg
  %ril.0.in.i = getelementptr inbounds [31 x i32]* %quant26bt_pos.quant26bt_neg.i, i32 0, i32 %mil.0.lcssa.i
  %ril.0.i = load i32* %ril.0.in.i, align 4
  store i32 %ril.0.i, i32* @il, align 4, !tbaa !1
  %120 = ashr i32 %ril.0.i, 2
  %121 = getelementptr inbounds [16 x i32]* @qq4_code4_table, i32 0, i32 %120
  %122 = load i32* %121, align 4, !tbaa !1
  %123 = mul nsw i32 %122, %108
  %124 = ashr i32 %123, 15
  store i32 %124, i32* @dlt, align 4, !tbaa !1
  %125 = load i32* @nbl, align 4, !tbaa !1
  %126 = mul nsw i32 %125, 127
  %127 = ashr i32 %126, 7
  %128 = getelementptr inbounds [16 x i32]* @wl_code_table, i32 0, i32 %120
  %129 = load i32* %128, align 4, !tbaa !1
  %130 = add nsw i32 %127, %129
  %131 = icmp slt i32 %130, 0
  %..i12 = select i1 %131, i32 0, i32 %130
  %132 = icmp sgt i32 %..i12, 18432
  %...i13 = select i1 %132, i32 18432, i32 %..i12
  store i32 %...i13, i32* @nbl, align 4, !tbaa !1
  %133 = lshr i32 %...i13, 6
  %134 = and i32 %133, 31
  %135 = ashr i32 %...i13, 11
  %136 = getelementptr inbounds [32 x i32]* @ilb_table, i32 0, i32 %134
  %137 = load i32* %136, align 4, !tbaa !1
  %138 = sub i32 9, %135
  %139 = ashr i32 %137, %138
  %140 = shl i32 %139, 3
  store i32 %140, i32* @detl, align 4, !tbaa !1
  %141 = add nsw i32 %98, %124
  store i32 %141, i32* @plt, align 4, !tbaa !1
  tail call void @upzero(i32 %124, i32* getelementptr inbounds ([6 x i32]* @delay_dltx, i32 0, i32 0), i32* getelementptr inbounds ([6 x i32]* @delay_bpl, i32 0, i32 0)) #4
  %142 = load i32* @al1, align 4, !tbaa !1
  %143 = load i32* @al2, align 4, !tbaa !1
  %144 = load i32* @plt, align 4, !tbaa !1
  %145 = load i32* @plt1, align 4, !tbaa !1
  %146 = load i32* @plt2, align 4, !tbaa !1
  %147 = shl nsw i32 %142, 2
  %148 = mul nsw i32 %145, %144
  %149 = icmp sgt i32 %148, -1
  %150 = sub nsw i32 0, %147
  %.1.i8 = select i1 %149, i32 %150, i32 %147
  %151 = ashr i32 %.1.i8, 7
  %152 = mul nsw i32 %146, %144
  %153 = ashr i32 %152, 31
  %154 = and i32 %153, -256
  %155 = or i32 %154, 128
  %156 = mul nsw i32 %143, 127
  %157 = ashr i32 %156, 7
  %wd4.0.i9 = add nsw i32 %151, %157
  %158 = add nsw i32 %wd4.0.i9, %155
  %159 = icmp sgt i32 %158, 12288
  %..i10 = select i1 %159, i32 12288, i32 %158
  %160 = icmp slt i32 %..i10, -12288
  %...i11 = select i1 %160, i32 -12288, i32 %..i10
  store i32 %...i11, i32* @al2, align 4, !tbaa !1
  %161 = mul nsw i32 %142, 255
  %162 = ashr i32 %161, 8
  %163 = ashr i32 %148, 31
  %164 = and i32 %163, -384
  %165 = add nsw i32 %162, 192
  %apl1.0.i5 = add nsw i32 %165, %164
  %166 = sub nsw i32 15360, %...i11
  %167 = icmp sgt i32 %apl1.0.i5, %166
  %.apl1.0.i6 = select i1 %167, i32 %166, i32 %apl1.0.i5
  %168 = sub nsw i32 0, %166
  %169 = icmp slt i32 %.apl1.0.i6, %168
  %apl1.2.i7 = select i1 %169, i32 %168, i32 %.apl1.0.i6
  store i32 %apl1.2.i7, i32* @al1, align 4, !tbaa !1
  %170 = load i32* @sl, align 4, !tbaa !1
  %171 = load i32* @dlt, align 4, !tbaa !1
  %172 = add nsw i32 %171, %170
  store i32 %172, i32* @rlt, align 4, !tbaa !1
  %173 = load i32* @rlt1, align 4, !tbaa !1
  store i32 %173, i32* @rlt2, align 4, !tbaa !1
  store i32 %172, i32* @rlt1, align 4, !tbaa !1
  store i32 %145, i32* @plt2, align 4, !tbaa !1
  store i32 %144, i32* @plt1, align 4, !tbaa !1
  %174 = load i32* getelementptr inbounds ([6 x i32]* @delay_bph, i32 0, i32 0), align 4, !tbaa !1
  %175 = load i32* getelementptr inbounds ([6 x i32]* @delay_dhx, i32 0, i32 0), align 4, !tbaa !1
  %176 = mul nsw i32 %175, %174
  %177 = load i32* getelementptr inbounds ([6 x i32]* @delay_bph, i32 0, i32 1), align 4, !tbaa !1
  %178 = load i32* getelementptr inbounds ([6 x i32]* @delay_dhx, i32 0, i32 1), align 4, !tbaa !1
  %179 = mul nsw i32 %178, %177
  %180 = add nsw i32 %179, %176
  %181 = load i32* getelementptr inbounds ([6 x i32]* @delay_bph, i32 0, i32 2), align 4, !tbaa !1
  %182 = load i32* getelementptr inbounds ([6 x i32]* @delay_dhx, i32 0, i32 2), align 4, !tbaa !1
  %183 = mul nsw i32 %182, %181
  %184 = add nsw i32 %180, %183
  %185 = load i32* getelementptr inbounds ([6 x i32]* @delay_bph, i32 0, i32 3), align 4, !tbaa !1
  %186 = load i32* getelementptr inbounds ([6 x i32]* @delay_dhx, i32 0, i32 3), align 4, !tbaa !1
  %187 = mul nsw i32 %186, %185
  %188 = add nsw i32 %184, %187
  %189 = load i32* getelementptr inbounds ([6 x i32]* @delay_bph, i32 0, i32 4), align 4, !tbaa !1
  %190 = load i32* getelementptr inbounds ([6 x i32]* @delay_dhx, i32 0, i32 4), align 4, !tbaa !1
  %191 = mul nsw i32 %190, %189
  %192 = add nsw i32 %188, %191
  %193 = load i32* getelementptr inbounds ([6 x i32]* @delay_bph, i32 0, i32 5), align 4, !tbaa !1
  %194 = load i32* getelementptr inbounds ([6 x i32]* @delay_dhx, i32 0, i32 5), align 4, !tbaa !1
  %195 = mul nsw i32 %194, %193
  %196 = add nsw i32 %192, %195
  %197 = ashr i32 %196, 14
  store i32 %197, i32* @szh, align 4, !tbaa !1
  %198 = load i32* @rh1, align 4, !tbaa !1
  %199 = load i32* @ah1, align 4, !tbaa !1
  %200 = load i32* @rh2, align 4, !tbaa !1
  %201 = load i32* @ah2, align 4, !tbaa !1
  %202 = mul i32 %199, %198
  %203 = mul i32 %201, %200
  %tmp.i3 = add i32 %203, %202
  %tmp1.i4 = shl i32 %tmp.i3, 1
  %204 = ashr i32 %tmp1.i4, 15
  store i32 %204, i32* @sph, align 4, !tbaa !1
  %205 = add nsw i32 %204, %197
  store i32 %205, i32* @sh, align 4, !tbaa !1
  %206 = load i32* @xh, align 4, !tbaa !1
  %207 = sub nsw i32 %206, %205
  store i32 %207, i32* @eh, align 4, !tbaa !1
  %208 = ashr i32 %207, 31
  %209 = and i32 %208, -2
  %210 = add nsw i32 %209, 3
  store i32 %210, i32* @ih, align 4, !tbaa !1
  %211 = load i32* @deth, align 4, !tbaa !1
  %212 = mul nsw i32 %211, 564
  %213 = ashr i32 %212, 12
  %214 = icmp sgt i32 %207, -1
  %215 = sub nsw i32 0, %207
  %m.0.i = select i1 %214, i32 %207, i32 %215
  %216 = icmp sgt i32 %m.0.i, %213
  br i1 %216, label %217, label %219

; <label>:217                                     ; preds = %quantl.exit
  %218 = add nsw i32 %209, 2
  store i32 %218, i32* @ih, align 4, !tbaa !1
  br label %219

; <label>:219                                     ; preds = %217, %quantl.exit
  %220 = phi i32 [ %218, %217 ], [ %210, %quantl.exit ]
  %221 = getelementptr inbounds [4 x i32]* @qq2_code2_table, i32 0, i32 %220
  %222 = load i32* %221, align 4, !tbaa !1
  %223 = mul nsw i32 %222, %211
  %224 = ashr i32 %223, 15
  store i32 %224, i32* @dh, align 4, !tbaa !1
  %225 = load i32* @nbh, align 4, !tbaa !1
  %226 = mul nsw i32 %225, 127
  %227 = ashr i32 %226, 7
  %228 = getelementptr inbounds [4 x i32]* @wh_code_table, i32 0, i32 %220
  %229 = load i32* %228, align 4, !tbaa !1
  %230 = add nsw i32 %227, %229
  %231 = icmp slt i32 %230, 0
  %..i1 = select i1 %231, i32 0, i32 %230
  %232 = icmp sgt i32 %..i1, 22528
  %...i2 = select i1 %232, i32 22528, i32 %..i1
  store i32 %...i2, i32* @nbh, align 4, !tbaa !1
  %233 = lshr i32 %...i2, 6
  %234 = and i32 %233, 31
  %235 = ashr i32 %...i2, 11
  %236 = getelementptr inbounds [32 x i32]* @ilb_table, i32 0, i32 %234
  %237 = load i32* %236, align 4, !tbaa !1
  %238 = sub i32 11, %235
  %239 = ashr i32 %237, %238
  %240 = shl i32 %239, 3
  store i32 %240, i32* @deth, align 4, !tbaa !1
  %241 = add nsw i32 %197, %224
  store i32 %241, i32* @ph, align 4, !tbaa !1
  tail call void @upzero(i32 %224, i32* getelementptr inbounds ([6 x i32]* @delay_dhx, i32 0, i32 0), i32* getelementptr inbounds ([6 x i32]* @delay_bph, i32 0, i32 0)) #4
  %242 = load i32* @ah1, align 4, !tbaa !1
  %243 = load i32* @ah2, align 4, !tbaa !1
  %244 = load i32* @ph, align 4, !tbaa !1
  %245 = load i32* @ph1, align 4, !tbaa !1
  %246 = load i32* @ph2, align 4, !tbaa !1
  %247 = shl nsw i32 %242, 2
  %248 = mul nsw i32 %245, %244
  %249 = icmp sgt i32 %248, -1
  %250 = sub nsw i32 0, %247
  %.1.i = select i1 %249, i32 %250, i32 %247
  %251 = ashr i32 %.1.i, 7
  %252 = mul nsw i32 %246, %244
  %253 = ashr i32 %252, 31
  %254 = and i32 %253, -256
  %255 = or i32 %254, 128
  %256 = mul nsw i32 %243, 127
  %257 = ashr i32 %256, 7
  %wd4.0.i = add nsw i32 %251, %257
  %258 = add nsw i32 %wd4.0.i, %255
  %259 = icmp sgt i32 %258, 12288
  %..i = select i1 %259, i32 12288, i32 %258
  %260 = icmp slt i32 %..i, -12288
  %...i = select i1 %260, i32 -12288, i32 %..i
  store i32 %...i, i32* @ah2, align 4, !tbaa !1
  %261 = mul nsw i32 %242, 255
  %262 = ashr i32 %261, 8
  %263 = ashr i32 %248, 31
  %264 = and i32 %263, -384
  %265 = add nsw i32 %262, 192
  %apl1.0.i = add nsw i32 %265, %264
  %266 = sub nsw i32 15360, %...i
  %267 = icmp sgt i32 %apl1.0.i, %266
  %.apl1.0.i = select i1 %267, i32 %266, i32 %apl1.0.i
  %268 = sub nsw i32 0, %266
  %269 = icmp slt i32 %.apl1.0.i, %268
  %apl1.2.i = select i1 %269, i32 %268, i32 %.apl1.0.i
  store i32 %apl1.2.i, i32* @ah1, align 4, !tbaa !1
  %270 = load i32* @sh, align 4, !tbaa !1
  %271 = load i32* @dh, align 4, !tbaa !1
  %272 = add nsw i32 %271, %270
  store i32 %272, i32* @yh, align 4, !tbaa !1
  %273 = load i32* @rh1, align 4, !tbaa !1
  store i32 %273, i32* @rh2, align 4, !tbaa !1
  store i32 %272, i32* @rh1, align 4, !tbaa !1
  store i32 %245, i32* @ph2, align 4, !tbaa !1
  store i32 %244, i32* @ph1, align 4, !tbaa !1
  %274 = load i32* @il, align 4, !tbaa !1
  %275 = load i32* @ih, align 4, !tbaa !1
  %276 = shl i32 %275, 6
  %277 = or i32 %276, %274
  ret i32 %277
}

; Function Attrs: nounwind readonly
define i32 @filtez(i32* nocapture readonly %bpl, i32* nocapture readonly %dlt) #2 {
  %1 = load i32* %bpl, align 4, !tbaa !1
  %2 = load i32* %dlt, align 4, !tbaa !1
  %3 = mul nsw i32 %2, %1
  %.06 = getelementptr inbounds i32* %dlt, i32 1
  %.017 = getelementptr inbounds i32* %bpl, i32 1
  %4 = load i32* %.017, align 4, !tbaa !1
  %5 = load i32* %.06, align 4, !tbaa !1
  %6 = mul nsw i32 %5, %4
  %7 = add nsw i32 %6, %3
  %.06.1 = getelementptr inbounds i32* %dlt, i32 2
  %.017.1 = getelementptr inbounds i32* %bpl, i32 2
  %8 = load i32* %.017.1, align 4, !tbaa !1
  %9 = load i32* %.06.1, align 4, !tbaa !1
  %10 = mul nsw i32 %9, %8
  %11 = add nsw i32 %10, %7
  %.06.2 = getelementptr inbounds i32* %dlt, i32 3
  %.017.2 = getelementptr inbounds i32* %bpl, i32 3
  %12 = load i32* %.017.2, align 4, !tbaa !1
  %13 = load i32* %.06.2, align 4, !tbaa !1
  %14 = mul nsw i32 %13, %12
  %15 = add nsw i32 %14, %11
  %.06.3 = getelementptr inbounds i32* %dlt, i32 4
  %.017.3 = getelementptr inbounds i32* %bpl, i32 4
  %16 = load i32* %.017.3, align 4, !tbaa !1
  %17 = load i32* %.06.3, align 4, !tbaa !1
  %18 = mul nsw i32 %17, %16
  %19 = add nsw i32 %18, %15
  %.06.4 = getelementptr inbounds i32* %dlt, i32 5
  %.017.4 = getelementptr inbounds i32* %bpl, i32 5
  %20 = load i32* %.017.4, align 4, !tbaa !1
  %21 = load i32* %.06.4, align 4, !tbaa !1
  %22 = mul nsw i32 %21, %20
  %23 = add nsw i32 %22, %19
  %24 = ashr i32 %23, 14
  ret i32 %24
}

; Function Attrs: nounwind readnone
define i32 @filtep(i32 %rlt1, i32 %al1, i32 %rlt2, i32 %al2) #0 {
  %1 = mul i32 %al1, %rlt1
  %2 = mul i32 %al2, %rlt2
  %tmp = add i32 %2, %1
  %tmp1 = shl i32 %tmp, 1
  %3 = ashr i32 %tmp1, 15
  ret i32 %3
}

; Function Attrs: nounwind readnone
define i32 @quantl(i32 %el, i32 %detl) #0 {
  %1 = icmp sgt i32 %el, -1
  %2 = sub nsw i32 0, %el
  %m.0.i = select i1 %1, i32 %el, i32 %2
  br label %5

; <label>:3                                       ; preds = %5
  %4 = icmp slt i32 %11, 30
  br i1 %4, label %5, label %12

; <label>:5                                       ; preds = %3, %0
  %mil.01 = phi i32 [ 0, %0 ], [ %11, %3 ]
  %6 = getelementptr inbounds [30 x i32]* @decis_levl, i32 0, i32 %mil.01
  %7 = load i32* %6, align 4, !tbaa !1
  %8 = mul nsw i32 %7, %detl
  %9 = ashr i32 %8, 15
  %10 = icmp sgt i32 %m.0.i, %9
  %11 = add nsw i32 %mil.01, 1
  br i1 %10, label %3, label %12

; <label>:12                                      ; preds = %5, %3
  %mil.0.lcssa = phi i32 [ %mil.01, %5 ], [ %11, %3 ]
  %quant26bt_pos.quant26bt_neg = select i1 %1, [31 x i32]* @quant26bt_pos, [31 x i32]* @quant26bt_neg
  %ril.0.in = getelementptr inbounds [31 x i32]* %quant26bt_pos.quant26bt_neg, i32 0, i32 %mil.0.lcssa
  %ril.0 = load i32* %ril.0.in, align 4
  ret i32 %ril.0
}

; Function Attrs: nounwind readnone
define i32 @logscl(i32 %il, i32 %nbl) #0 {
  %1 = mul nsw i32 %nbl, 127
  %2 = ashr i32 %1, 7
  %3 = ashr i32 %il, 2
  %4 = getelementptr inbounds [16 x i32]* @wl_code_table, i32 0, i32 %3
  %5 = load i32* %4, align 4, !tbaa !1
  %6 = add nsw i32 %5, %2
  %7 = icmp slt i32 %6, 0
  %. = select i1 %7, i32 0, i32 %6
  %8 = icmp sgt i32 %., 18432
  %.. = select i1 %8, i32 18432, i32 %.
  ret i32 %..
}

; Function Attrs: nounwind readnone
define i32 @scalel(i32 %nbl, i32 %shift_constant) #0 {
  %1 = lshr i32 %nbl, 6
  %2 = and i32 %1, 31
  %3 = ashr i32 %nbl, 11
  %4 = getelementptr inbounds [32 x i32]* @ilb_table, i32 0, i32 %2
  %5 = load i32* %4, align 4, !tbaa !1
  %6 = add nsw i32 %shift_constant, 1
  %7 = sub i32 %6, %3
  %8 = ashr i32 %5, %7
  %9 = shl i32 %8, 3
  ret i32 %9
}

; Function Attrs: nounwind
define void @upzero(i32 %dlt, i32* nocapture %dlti, i32* nocapture %bli) #1 {
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

; Function Attrs: nounwind readnone
define i32 @uppol2(i32 %al1, i32 %al2, i32 %plt, i32 %plt1, i32 %plt2) #0 {
  %1 = shl nsw i32 %al1, 2
  %2 = mul nsw i32 %plt1, %plt
  %3 = icmp sgt i32 %2, -1
  %4 = sub nsw i32 0, %1
  %.1 = select i1 %3, i32 %4, i32 %1
  %5 = ashr i32 %.1, 7
  %6 = mul nsw i32 %plt2, %plt
  %7 = ashr i32 %6, 31
  %8 = and i32 %7, -256
  %9 = or i32 %8, 128
  %10 = mul nsw i32 %al2, 127
  %11 = ashr i32 %10, 7
  %wd4.0 = add nsw i32 %5, %11
  %12 = add nsw i32 %wd4.0, %9
  %13 = icmp sgt i32 %12, 12288
  %. = select i1 %13, i32 12288, i32 %12
  %14 = icmp slt i32 %., -12288
  %.. = select i1 %14, i32 -12288, i32 %.
  ret i32 %..
}

; Function Attrs: nounwind readnone
define i32 @uppol1(i32 %al1, i32 %apl2, i32 %plt, i32 %plt1) #0 {
  %1 = mul nsw i32 %al1, 255
  %2 = ashr i32 %1, 8
  %3 = mul nsw i32 %plt1, %plt
  %4 = ashr i32 %3, 31
  %5 = and i32 %4, -384
  %6 = add nsw i32 %2, 192
  %apl1.0 = add nsw i32 %6, %5
  %7 = sub nsw i32 15360, %apl2
  %8 = icmp sgt i32 %apl1.0, %7
  %.apl1.0 = select i1 %8, i32 %7, i32 %apl1.0
  %9 = sub nsw i32 0, %7
  %10 = icmp slt i32 %.apl1.0, %9
  %apl1.2 = select i1 %10, i32 %9, i32 %.apl1.0
  ret i32 %apl1.2
}

; Function Attrs: nounwind readnone
define i32 @logsch(i32 %ih, i32 %nbh) #0 {
  %1 = mul nsw i32 %nbh, 127
  %2 = ashr i32 %1, 7
  %3 = getelementptr inbounds [4 x i32]* @wh_code_table, i32 0, i32 %ih
  %4 = load i32* %3, align 4, !tbaa !1
  %5 = add nsw i32 %4, %2
  %6 = icmp slt i32 %5, 0
  %. = select i1 %6, i32 0, i32 %5
  %7 = icmp sgt i32 %., 22528
  %.. = select i1 %7, i32 22528, i32 %.
  ret i32 %..
}

; Function Attrs: nounwind
define void @decode(i32 %input) #1 {
  %1 = and i32 %input, 63
  store i32 %1, i32* @ilr, align 4, !tbaa !1
  %2 = ashr i32 %input, 6
  store i32 %2, i32* @ih, align 4, !tbaa !1
  %3 = load i32* getelementptr inbounds ([6 x i32]* @dec_del_bpl, i32 0, i32 0), align 4, !tbaa !1
  %4 = load i32* getelementptr inbounds ([6 x i32]* @dec_del_dltx, i32 0, i32 0), align 4, !tbaa !1
  %5 = mul nsw i32 %4, %3
  %6 = load i32* getelementptr inbounds ([6 x i32]* @dec_del_bpl, i32 0, i32 1), align 4, !tbaa !1
  %7 = load i32* getelementptr inbounds ([6 x i32]* @dec_del_dltx, i32 0, i32 1), align 4, !tbaa !1
  %8 = mul nsw i32 %7, %6
  %9 = add nsw i32 %8, %5
  %10 = load i32* getelementptr inbounds ([6 x i32]* @dec_del_bpl, i32 0, i32 2), align 4, !tbaa !1
  %11 = load i32* getelementptr inbounds ([6 x i32]* @dec_del_dltx, i32 0, i32 2), align 4, !tbaa !1
  %12 = mul nsw i32 %11, %10
  %13 = add nsw i32 %9, %12
  %14 = load i32* getelementptr inbounds ([6 x i32]* @dec_del_bpl, i32 0, i32 3), align 4, !tbaa !1
  %15 = load i32* getelementptr inbounds ([6 x i32]* @dec_del_dltx, i32 0, i32 3), align 4, !tbaa !1
  %16 = mul nsw i32 %15, %14
  %17 = add nsw i32 %13, %16
  %18 = load i32* getelementptr inbounds ([6 x i32]* @dec_del_bpl, i32 0, i32 4), align 4, !tbaa !1
  %19 = load i32* getelementptr inbounds ([6 x i32]* @dec_del_dltx, i32 0, i32 4), align 4, !tbaa !1
  %20 = mul nsw i32 %19, %18
  %21 = add nsw i32 %17, %20
  %22 = load i32* getelementptr inbounds ([6 x i32]* @dec_del_bpl, i32 0, i32 5), align 4, !tbaa !1
  %23 = load i32* getelementptr inbounds ([6 x i32]* @dec_del_dltx, i32 0, i32 5), align 4, !tbaa !1
  %24 = mul nsw i32 %23, %22
  %25 = add nsw i32 %21, %24
  %26 = ashr i32 %25, 14
  store i32 %26, i32* @dec_szl, align 4, !tbaa !1
  %27 = load i32* @dec_rlt1, align 4, !tbaa !1
  %28 = load i32* @dec_al1, align 4, !tbaa !1
  %29 = load i32* @dec_rlt2, align 4, !tbaa !1
  %30 = load i32* @dec_al2, align 4, !tbaa !1
  %31 = mul i32 %28, %27
  %32 = mul i32 %30, %29
  %tmp.i = add i32 %32, %31
  %tmp1.i = shl i32 %tmp.i, 1
  %33 = ashr i32 %tmp1.i, 15
  store i32 %33, i32* @dec_spl, align 4, !tbaa !1
  %34 = add nsw i32 %33, %26
  store i32 %34, i32* @dec_sl, align 4, !tbaa !1
  %35 = load i32* @dec_detl, align 4, !tbaa !1
  %36 = lshr i32 %1, 2
  %37 = getelementptr inbounds [16 x i32]* @qq4_code4_table, i32 0, i32 %36
  %38 = load i32* %37, align 4, !tbaa !1
  %39 = mul nsw i32 %38, %35
  %40 = ashr i32 %39, 15
  store i32 %40, i32* @dec_dlt, align 4, !tbaa !1
  %41 = load i32* @il, align 4, !tbaa !1
  %42 = getelementptr inbounds [64 x i32]* @qq6_code6_table, i32 0, i32 %41
  %43 = load i32* %42, align 4, !tbaa !1
  %44 = mul nsw i32 %43, %35
  %45 = ashr i32 %44, 15
  store i32 %45, i32* @dl, align 4, !tbaa !1
  %46 = add nsw i32 %45, %34
  store i32 %46, i32* @rl, align 4, !tbaa !1
  %47 = load i32* @dec_nbl, align 4, !tbaa !1
  %48 = mul nsw i32 %47, 127
  %49 = ashr i32 %48, 7
  %50 = getelementptr inbounds [16 x i32]* @wl_code_table, i32 0, i32 %36
  %51 = load i32* %50, align 4, !tbaa !1
  %52 = add nsw i32 %49, %51
  %53 = icmp slt i32 %52, 0
  %..i12 = select i1 %53, i32 0, i32 %52
  %54 = icmp sgt i32 %..i12, 18432
  %...i13 = select i1 %54, i32 18432, i32 %..i12
  store i32 %...i13, i32* @dec_nbl, align 4, !tbaa !1
  %55 = lshr i32 %...i13, 6
  %56 = and i32 %55, 31
  %57 = ashr i32 %...i13, 11
  %58 = getelementptr inbounds [32 x i32]* @ilb_table, i32 0, i32 %56
  %59 = load i32* %58, align 4, !tbaa !1
  %60 = sub i32 9, %57
  %61 = ashr i32 %59, %60
  %62 = shl i32 %61, 3
  store i32 %62, i32* @dec_detl, align 4, !tbaa !1
  %63 = add nsw i32 %26, %40
  store i32 %63, i32* @dec_plt, align 4, !tbaa !1
  tail call void @upzero(i32 %40, i32* getelementptr inbounds ([6 x i32]* @dec_del_dltx, i32 0, i32 0), i32* getelementptr inbounds ([6 x i32]* @dec_del_bpl, i32 0, i32 0)) #4
  %64 = load i32* @dec_al1, align 4, !tbaa !1
  %65 = load i32* @dec_al2, align 4, !tbaa !1
  %66 = load i32* @dec_plt, align 4, !tbaa !1
  %67 = load i32* @dec_plt1, align 4, !tbaa !1
  %68 = load i32* @dec_plt2, align 4, !tbaa !1
  %69 = shl nsw i32 %64, 2
  %70 = mul nsw i32 %67, %66
  %71 = icmp sgt i32 %70, -1
  %72 = sub nsw i32 0, %69
  %.1.i8 = select i1 %71, i32 %72, i32 %69
  %73 = ashr i32 %.1.i8, 7
  %74 = mul nsw i32 %68, %66
  %75 = ashr i32 %74, 31
  %76 = and i32 %75, -256
  %77 = or i32 %76, 128
  %78 = mul nsw i32 %65, 127
  %79 = ashr i32 %78, 7
  %wd4.0.i9 = add nsw i32 %73, %79
  %80 = add nsw i32 %wd4.0.i9, %77
  %81 = icmp sgt i32 %80, 12288
  %..i10 = select i1 %81, i32 12288, i32 %80
  %82 = icmp slt i32 %..i10, -12288
  %...i11 = select i1 %82, i32 -12288, i32 %..i10
  store i32 %...i11, i32* @dec_al2, align 4, !tbaa !1
  %83 = mul nsw i32 %64, 255
  %84 = ashr i32 %83, 8
  %85 = ashr i32 %70, 31
  %86 = and i32 %85, -384
  %87 = add nsw i32 %84, 192
  %apl1.0.i5 = add nsw i32 %87, %86
  %88 = sub nsw i32 15360, %...i11
  %89 = icmp sgt i32 %apl1.0.i5, %88
  %.apl1.0.i6 = select i1 %89, i32 %88, i32 %apl1.0.i5
  %90 = sub nsw i32 0, %88
  %91 = icmp slt i32 %.apl1.0.i6, %90
  %apl1.2.i7 = select i1 %91, i32 %90, i32 %.apl1.0.i6
  store i32 %apl1.2.i7, i32* @dec_al1, align 4, !tbaa !1
  %92 = load i32* @dec_sl, align 4, !tbaa !1
  %93 = load i32* @dec_dlt, align 4, !tbaa !1
  %94 = add nsw i32 %93, %92
  store i32 %94, i32* @dec_rlt, align 4, !tbaa !1
  %95 = load i32* @dec_rlt1, align 4, !tbaa !1
  store i32 %95, i32* @dec_rlt2, align 4, !tbaa !1
  store i32 %94, i32* @dec_rlt1, align 4, !tbaa !1
  store i32 %67, i32* @dec_plt2, align 4, !tbaa !1
  store i32 %66, i32* @dec_plt1, align 4, !tbaa !1
  %96 = load i32* getelementptr inbounds ([6 x i32]* @dec_del_bph, i32 0, i32 0), align 4, !tbaa !1
  %97 = load i32* getelementptr inbounds ([6 x i32]* @dec_del_dhx, i32 0, i32 0), align 4, !tbaa !1
  %98 = mul nsw i32 %97, %96
  %99 = load i32* getelementptr inbounds ([6 x i32]* @dec_del_bph, i32 0, i32 1), align 4, !tbaa !1
  %100 = load i32* getelementptr inbounds ([6 x i32]* @dec_del_dhx, i32 0, i32 1), align 4, !tbaa !1
  %101 = mul nsw i32 %100, %99
  %102 = add nsw i32 %101, %98
  %103 = load i32* getelementptr inbounds ([6 x i32]* @dec_del_bph, i32 0, i32 2), align 4, !tbaa !1
  %104 = load i32* getelementptr inbounds ([6 x i32]* @dec_del_dhx, i32 0, i32 2), align 4, !tbaa !1
  %105 = mul nsw i32 %104, %103
  %106 = add nsw i32 %102, %105
  %107 = load i32* getelementptr inbounds ([6 x i32]* @dec_del_bph, i32 0, i32 3), align 4, !tbaa !1
  %108 = load i32* getelementptr inbounds ([6 x i32]* @dec_del_dhx, i32 0, i32 3), align 4, !tbaa !1
  %109 = mul nsw i32 %108, %107
  %110 = add nsw i32 %106, %109
  %111 = load i32* getelementptr inbounds ([6 x i32]* @dec_del_bph, i32 0, i32 4), align 4, !tbaa !1
  %112 = load i32* getelementptr inbounds ([6 x i32]* @dec_del_dhx, i32 0, i32 4), align 4, !tbaa !1
  %113 = mul nsw i32 %112, %111
  %114 = add nsw i32 %110, %113
  %115 = load i32* getelementptr inbounds ([6 x i32]* @dec_del_bph, i32 0, i32 5), align 4, !tbaa !1
  %116 = load i32* getelementptr inbounds ([6 x i32]* @dec_del_dhx, i32 0, i32 5), align 4, !tbaa !1
  %117 = mul nsw i32 %116, %115
  %118 = add nsw i32 %114, %117
  %119 = ashr i32 %118, 14
  store i32 %119, i32* @dec_szh, align 4, !tbaa !1
  %120 = load i32* @dec_rh1, align 4, !tbaa !1
  %121 = load i32* @dec_ah1, align 4, !tbaa !1
  %122 = load i32* @dec_rh2, align 4, !tbaa !1
  %123 = load i32* @dec_ah2, align 4, !tbaa !1
  %124 = mul i32 %121, %120
  %125 = mul i32 %123, %122
  %tmp.i3 = add i32 %125, %124
  %tmp1.i4 = shl i32 %tmp.i3, 1
  %126 = ashr i32 %tmp1.i4, 15
  store i32 %126, i32* @dec_sph, align 4, !tbaa !1
  %127 = add nsw i32 %126, %119
  store i32 %127, i32* @dec_sh, align 4, !tbaa !1
  %128 = load i32* @dec_deth, align 4, !tbaa !1
  %129 = load i32* @ih, align 4, !tbaa !1
  %130 = getelementptr inbounds [4 x i32]* @qq2_code2_table, i32 0, i32 %129
  %131 = load i32* %130, align 4, !tbaa !1
  %132 = mul nsw i32 %131, %128
  %133 = ashr i32 %132, 15
  store i32 %133, i32* @dec_dh, align 4, !tbaa !1
  %134 = load i32* @dec_nbh, align 4, !tbaa !1
  %135 = mul nsw i32 %134, 127
  %136 = ashr i32 %135, 7
  %137 = getelementptr inbounds [4 x i32]* @wh_code_table, i32 0, i32 %129
  %138 = load i32* %137, align 4, !tbaa !1
  %139 = add nsw i32 %136, %138
  %140 = icmp slt i32 %139, 0
  %..i1 = select i1 %140, i32 0, i32 %139
  %141 = icmp sgt i32 %..i1, 22528
  %...i2 = select i1 %141, i32 22528, i32 %..i1
  store i32 %...i2, i32* @dec_nbh, align 4, !tbaa !1
  %142 = lshr i32 %...i2, 6
  %143 = and i32 %142, 31
  %144 = ashr i32 %...i2, 11
  %145 = getelementptr inbounds [32 x i32]* @ilb_table, i32 0, i32 %143
  %146 = load i32* %145, align 4, !tbaa !1
  %147 = sub i32 11, %144
  %148 = ashr i32 %146, %147
  %149 = shl i32 %148, 3
  store i32 %149, i32* @dec_deth, align 4, !tbaa !1
  %150 = add nsw i32 %119, %133
  store i32 %150, i32* @dec_ph, align 4, !tbaa !1
  tail call void @upzero(i32 %133, i32* getelementptr inbounds ([6 x i32]* @dec_del_dhx, i32 0, i32 0), i32* getelementptr inbounds ([6 x i32]* @dec_del_bph, i32 0, i32 0)) #4
  %151 = load i32* @dec_ah1, align 4, !tbaa !1
  %152 = load i32* @dec_ah2, align 4, !tbaa !1
  %153 = load i32* @dec_ph, align 4, !tbaa !1
  %154 = load i32* @dec_ph1, align 4, !tbaa !1
  %155 = load i32* @dec_ph2, align 4, !tbaa !1
  %156 = shl nsw i32 %151, 2
  %157 = mul nsw i32 %154, %153
  %158 = icmp sgt i32 %157, -1
  %159 = sub nsw i32 0, %156
  %.1.i = select i1 %158, i32 %159, i32 %156
  %160 = ashr i32 %.1.i, 7
  %161 = mul nsw i32 %155, %153
  %162 = ashr i32 %161, 31
  %163 = and i32 %162, -256
  %164 = or i32 %163, 128
  %165 = mul nsw i32 %152, 127
  %166 = ashr i32 %165, 7
  %wd4.0.i = add nsw i32 %160, %166
  %167 = add nsw i32 %wd4.0.i, %164
  %168 = icmp sgt i32 %167, 12288
  %..i = select i1 %168, i32 12288, i32 %167
  %169 = icmp slt i32 %..i, -12288
  %...i = select i1 %169, i32 -12288, i32 %..i
  store i32 %...i, i32* @dec_ah2, align 4, !tbaa !1
  %170 = mul nsw i32 %151, 255
  %171 = ashr i32 %170, 8
  %172 = ashr i32 %157, 31
  %173 = and i32 %172, -384
  %174 = add nsw i32 %171, 192
  %apl1.0.i = add nsw i32 %174, %173
  %175 = sub nsw i32 15360, %...i
  %176 = icmp sgt i32 %apl1.0.i, %175
  %.apl1.0.i = select i1 %176, i32 %175, i32 %apl1.0.i
  %177 = sub nsw i32 0, %175
  %178 = icmp slt i32 %.apl1.0.i, %177
  %apl1.2.i = select i1 %178, i32 %177, i32 %.apl1.0.i
  store i32 %apl1.2.i, i32* @dec_ah1, align 4, !tbaa !1
  %179 = load i32* @dec_sh, align 4, !tbaa !1
  %180 = load i32* @dec_dh, align 4, !tbaa !1
  %181 = add nsw i32 %180, %179
  store i32 %181, i32* @rh, align 4, !tbaa !1
  %182 = load i32* @dec_rh1, align 4, !tbaa !1
  store i32 %182, i32* @dec_rh2, align 4, !tbaa !1
  store i32 %181, i32* @dec_rh1, align 4, !tbaa !1
  store i32 %154, i32* @dec_ph2, align 4, !tbaa !1
  store i32 %153, i32* @dec_ph1, align 4, !tbaa !1
  %183 = load i32* @rl, align 4, !tbaa !1
  %184 = sub nsw i32 %183, %181
  store i32 %184, i32* @xd, align 4, !tbaa !1
  %185 = add nsw i32 %181, %183
  store i32 %185, i32* @xs, align 4, !tbaa !1
  %186 = mul nsw i32 %184, 12
  %187 = mul nsw i32 %185, -44
  %188 = load i32* getelementptr inbounds ([11 x i32]* @accumc, i32 0, i32 0), align 4, !tbaa !1
  %189 = mul nsw i32 %188, -44
  %190 = add nsw i32 %189, %186
  %191 = load i32* getelementptr inbounds ([11 x i32]* @accumd, i32 0, i32 0), align 4, !tbaa !1
  %192 = mul nsw i32 %191, 212
  %193 = add nsw i32 %192, %187
  %194 = load i32* getelementptr inbounds ([11 x i32]* @accumc, i32 0, i32 1), align 4, !tbaa !1
  %195 = mul nsw i32 %194, 48
  %196 = add nsw i32 %195, %190
  %197 = load i32* getelementptr inbounds ([11 x i32]* @accumd, i32 0, i32 1), align 4, !tbaa !1
  %198 = mul nsw i32 %197, -624
  %199 = add nsw i32 %198, %193
  %200 = load i32* getelementptr inbounds ([11 x i32]* @accumc, i32 0, i32 2), align 4, !tbaa !1
  %201 = shl nsw i32 %200, 7
  %202 = add nsw i32 %201, %196
  %203 = load i32* getelementptr inbounds ([11 x i32]* @accumd, i32 0, i32 2), align 4, !tbaa !1
  %204 = mul nsw i32 %203, 1448
  %205 = add nsw i32 %204, %199
  %206 = load i32* getelementptr inbounds ([11 x i32]* @accumc, i32 0, i32 3), align 4, !tbaa !1
  %207 = mul nsw i32 %206, -840
  %208 = add nsw i32 %207, %202
  %209 = load i32* getelementptr inbounds ([11 x i32]* @accumd, i32 0, i32 3), align 4, !tbaa !1
  %210 = mul nsw i32 %209, -3220
  %211 = add nsw i32 %210, %205
  %212 = load i32* getelementptr inbounds ([11 x i32]* @accumc, i32 0, i32 4), align 4, !tbaa !1
  %213 = mul nsw i32 %212, 3804
  %214 = add nsw i32 %213, %208
  %215 = load i32* getelementptr inbounds ([11 x i32]* @accumd, i32 0, i32 4), align 4, !tbaa !1
  %216 = mul nsw i32 %215, 15504
  %217 = add nsw i32 %216, %211
  %218 = load i32* getelementptr inbounds ([11 x i32]* @accumc, i32 0, i32 5), align 4, !tbaa !1
  %219 = mul nsw i32 %218, 15504
  %220 = add nsw i32 %219, %214
  %221 = load i32* getelementptr inbounds ([11 x i32]* @accumd, i32 0, i32 5), align 4, !tbaa !1
  %222 = mul nsw i32 %221, 3804
  %223 = add nsw i32 %222, %217
  %224 = load i32* getelementptr inbounds ([11 x i32]* @accumc, i32 0, i32 6), align 4, !tbaa !1
  %225 = mul nsw i32 %224, -3220
  %226 = add nsw i32 %225, %220
  %227 = load i32* getelementptr inbounds ([11 x i32]* @accumd, i32 0, i32 6), align 4, !tbaa !1
  %228 = mul nsw i32 %227, -840
  %229 = add nsw i32 %228, %223
  %230 = load i32* getelementptr inbounds ([11 x i32]* @accumc, i32 0, i32 7), align 4, !tbaa !1
  %231 = mul nsw i32 %230, 1448
  %232 = add nsw i32 %231, %226
  %233 = load i32* getelementptr inbounds ([11 x i32]* @accumd, i32 0, i32 7), align 4, !tbaa !1
  %234 = shl nsw i32 %233, 7
  %235 = add nsw i32 %234, %229
  %236 = load i32* getelementptr inbounds ([11 x i32]* @accumc, i32 0, i32 8), align 4, !tbaa !1
  %237 = mul nsw i32 %236, -624
  %238 = add nsw i32 %237, %232
  %239 = load i32* getelementptr inbounds ([11 x i32]* @accumd, i32 0, i32 8), align 4, !tbaa !1
  %240 = mul nsw i32 %239, 48
  %241 = add nsw i32 %240, %235
  %242 = load i32* getelementptr inbounds ([11 x i32]* @accumc, i32 0, i32 9), align 4, !tbaa !1
  %243 = mul nsw i32 %242, 212
  %244 = add nsw i32 %243, %238
  %245 = load i32* getelementptr inbounds ([11 x i32]* @accumd, i32 0, i32 9), align 4, !tbaa !1
  %246 = mul nsw i32 %245, -44
  %247 = add nsw i32 %246, %241
  %248 = load i32* getelementptr inbounds ([11 x i32]* @accumc, i32 0, i32 10), align 4, !tbaa !1
  %249 = mul nsw i32 %248, -44
  %250 = add nsw i32 %249, %244
  %251 = load i32* getelementptr inbounds ([11 x i32]* @accumd, i32 0, i32 10), align 4, !tbaa !1
  %252 = mul nsw i32 %251, 12
  %253 = add nsw i32 %252, %247
  %254 = ashr i32 %250, 14
  store i32 %254, i32* @xout1, align 4, !tbaa !1
  %255 = ashr i32 %253, 14
  store i32 %255, i32* @xout2, align 4, !tbaa !1
  store i32 %242, i32* getelementptr inbounds ([11 x i32]* @accumc, i32 0, i32 10), align 4, !tbaa !1
  store i32 %245, i32* getelementptr inbounds ([11 x i32]* @accumd, i32 0, i32 10), align 4, !tbaa !1
  store i32 %236, i32* getelementptr inbounds ([11 x i32]* @accumc, i32 0, i32 9), align 4, !tbaa !1
  store i32 %239, i32* getelementptr inbounds ([11 x i32]* @accumd, i32 0, i32 9), align 4, !tbaa !1
  store i32 %230, i32* getelementptr inbounds ([11 x i32]* @accumc, i32 0, i32 8), align 4, !tbaa !1
  store i32 %233, i32* getelementptr inbounds ([11 x i32]* @accumd, i32 0, i32 8), align 4, !tbaa !1
  store i32 %224, i32* getelementptr inbounds ([11 x i32]* @accumc, i32 0, i32 7), align 4, !tbaa !1
  store i32 %227, i32* getelementptr inbounds ([11 x i32]* @accumd, i32 0, i32 7), align 4, !tbaa !1
  store i32 %218, i32* getelementptr inbounds ([11 x i32]* @accumc, i32 0, i32 6), align 4, !tbaa !1
  store i32 %221, i32* getelementptr inbounds ([11 x i32]* @accumd, i32 0, i32 6), align 4, !tbaa !1
  store i32 %212, i32* getelementptr inbounds ([11 x i32]* @accumc, i32 0, i32 5), align 4, !tbaa !1
  store i32 %215, i32* getelementptr inbounds ([11 x i32]* @accumd, i32 0, i32 5), align 4, !tbaa !1
  store i32 %206, i32* getelementptr inbounds ([11 x i32]* @accumc, i32 0, i32 4), align 4, !tbaa !1
  store i32 %209, i32* getelementptr inbounds ([11 x i32]* @accumd, i32 0, i32 4), align 4, !tbaa !1
  store i32 %200, i32* getelementptr inbounds ([11 x i32]* @accumc, i32 0, i32 3), align 4, !tbaa !1
  store i32 %203, i32* getelementptr inbounds ([11 x i32]* @accumd, i32 0, i32 3), align 4, !tbaa !1
  store i32 %194, i32* getelementptr inbounds ([11 x i32]* @accumc, i32 0, i32 2), align 4, !tbaa !1
  store i32 %197, i32* getelementptr inbounds ([11 x i32]* @accumd, i32 0, i32 2), align 4, !tbaa !1
  store i32 %188, i32* getelementptr inbounds ([11 x i32]* @accumc, i32 0, i32 1), align 4, !tbaa !1
  store i32 %191, i32* getelementptr inbounds ([11 x i32]* @accumd, i32 0, i32 1), align 4, !tbaa !1
  store i32 %184, i32* getelementptr inbounds ([11 x i32]* @accumc, i32 0, i32 0), align 4, !tbaa !1
  store i32 %185, i32* getelementptr inbounds ([11 x i32]* @accumd, i32 0, i32 0), align 4, !tbaa !1
  ret void
}

; Function Attrs: nounwind
define void @reset() #1 {
.preheader29:
  store i32 32, i32* @dec_detl, align 4, !tbaa !1
  store i32 32, i32* @detl, align 4, !tbaa !1
  store i32 8, i32* @dec_deth, align 4, !tbaa !1
  store i32 8, i32* @deth, align 4, !tbaa !1
  store i32 0, i32* @rlt2, align 4, !tbaa !1
  store i32 0, i32* @rlt1, align 4, !tbaa !1
  store i32 0, i32* @plt2, align 4, !tbaa !1
  store i32 0, i32* @plt1, align 4, !tbaa !1
  store i32 0, i32* @al2, align 4, !tbaa !1
  store i32 0, i32* @al1, align 4, !tbaa !1
  store i32 0, i32* @nbl, align 4, !tbaa !1
  store i32 0, i32* @rh2, align 4, !tbaa !1
  store i32 0, i32* @rh1, align 4, !tbaa !1
  store i32 0, i32* @ph2, align 4, !tbaa !1
  store i32 0, i32* @ph1, align 4, !tbaa !1
  store i32 0, i32* @ah2, align 4, !tbaa !1
  store i32 0, i32* @ah1, align 4, !tbaa !1
  store i32 0, i32* @nbh, align 4, !tbaa !1
  store i32 0, i32* @dec_rlt2, align 4, !tbaa !1
  store i32 0, i32* @dec_rlt1, align 4, !tbaa !1
  store i32 0, i32* @dec_plt2, align 4, !tbaa !1
  store i32 0, i32* @dec_plt1, align 4, !tbaa !1
  store i32 0, i32* @dec_al2, align 4, !tbaa !1
  store i32 0, i32* @dec_al1, align 4, !tbaa !1
  store i32 0, i32* @dec_nbl, align 4, !tbaa !1
  store i32 0, i32* @dec_rh2, align 4, !tbaa !1
  store i32 0, i32* @dec_rh1, align 4, !tbaa !1
  store i32 0, i32* @dec_ph2, align 4, !tbaa !1
  store i32 0, i32* @dec_ph1, align 4, !tbaa !1
  store i32 0, i32* @dec_ah2, align 4, !tbaa !1
  store i32 0, i32* @dec_ah1, align 4, !tbaa !1
  store i32 0, i32* @dec_nbh, align 4, !tbaa !1
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
  store i32 0, i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 0), align 4, !tbaa !1
  store i32 0, i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 1), align 4, !tbaa !1
  store i32 0, i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 2), align 4, !tbaa !1
  store i32 0, i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 3), align 4, !tbaa !1
  store i32 0, i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 4), align 4, !tbaa !1
  store i32 0, i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 5), align 4, !tbaa !1
  store i32 0, i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 6), align 4, !tbaa !1
  store i32 0, i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 7), align 4, !tbaa !1
  store i32 0, i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 8), align 4, !tbaa !1
  store i32 0, i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 9), align 4, !tbaa !1
  store i32 0, i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 10), align 4, !tbaa !1
  store i32 0, i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 11), align 4, !tbaa !1
  store i32 0, i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 12), align 4, !tbaa !1
  store i32 0, i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 13), align 4, !tbaa !1
  store i32 0, i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 14), align 4, !tbaa !1
  store i32 0, i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 15), align 4, !tbaa !1
  store i32 0, i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 16), align 4, !tbaa !1
  store i32 0, i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 17), align 4, !tbaa !1
  store i32 0, i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 18), align 4, !tbaa !1
  store i32 0, i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 19), align 4, !tbaa !1
  store i32 0, i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 20), align 4, !tbaa !1
  store i32 0, i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 21), align 4, !tbaa !1
  store i32 0, i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 22), align 4, !tbaa !1
  store i32 0, i32* getelementptr inbounds ([24 x i32]* @tqmf, i32 0, i32 23), align 4, !tbaa !1
  store i32 0, i32* getelementptr inbounds ([11 x i32]* @accumc, i32 0, i32 0), align 4, !tbaa !1
  store i32 0, i32* getelementptr inbounds ([11 x i32]* @accumd, i32 0, i32 0), align 4, !tbaa !1
  store i32 0, i32* getelementptr inbounds ([11 x i32]* @accumc, i32 0, i32 1), align 4, !tbaa !1
  store i32 0, i32* getelementptr inbounds ([11 x i32]* @accumd, i32 0, i32 1), align 4, !tbaa !1
  store i32 0, i32* getelementptr inbounds ([11 x i32]* @accumc, i32 0, i32 2), align 4, !tbaa !1
  store i32 0, i32* getelementptr inbounds ([11 x i32]* @accumd, i32 0, i32 2), align 4, !tbaa !1
  store i32 0, i32* getelementptr inbounds ([11 x i32]* @accumc, i32 0, i32 3), align 4, !tbaa !1
  store i32 0, i32* getelementptr inbounds ([11 x i32]* @accumd, i32 0, i32 3), align 4, !tbaa !1
  store i32 0, i32* getelementptr inbounds ([11 x i32]* @accumc, i32 0, i32 4), align 4, !tbaa !1
  store i32 0, i32* getelementptr inbounds ([11 x i32]* @accumd, i32 0, i32 4), align 4, !tbaa !1
  store i32 0, i32* getelementptr inbounds ([11 x i32]* @accumc, i32 0, i32 5), align 4, !tbaa !1
  store i32 0, i32* getelementptr inbounds ([11 x i32]* @accumd, i32 0, i32 5), align 4, !tbaa !1
  store i32 0, i32* getelementptr inbounds ([11 x i32]* @accumc, i32 0, i32 6), align 4, !tbaa !1
  store i32 0, i32* getelementptr inbounds ([11 x i32]* @accumd, i32 0, i32 6), align 4, !tbaa !1
  store i32 0, i32* getelementptr inbounds ([11 x i32]* @accumc, i32 0, i32 7), align 4, !tbaa !1
  store i32 0, i32* getelementptr inbounds ([11 x i32]* @accumd, i32 0, i32 7), align 4, !tbaa !1
  store i32 0, i32* getelementptr inbounds ([11 x i32]* @accumc, i32 0, i32 8), align 4, !tbaa !1
  store i32 0, i32* getelementptr inbounds ([11 x i32]* @accumd, i32 0, i32 8), align 4, !tbaa !1
  store i32 0, i32* getelementptr inbounds ([11 x i32]* @accumc, i32 0, i32 9), align 4, !tbaa !1
  store i32 0, i32* getelementptr inbounds ([11 x i32]* @accumd, i32 0, i32 9), align 4, !tbaa !1
  store i32 0, i32* getelementptr inbounds ([11 x i32]* @accumc, i32 0, i32 10), align 4, !tbaa !1
  store i32 0, i32* getelementptr inbounds ([11 x i32]* @accumd, i32 0, i32 10), align 4, !tbaa !1
  ret void
}

; Function Attrs: nounwind
define void @adpcm_main() #1 {
  tail call void @reset() #4
  br label %1

; <label>:1                                       ; preds = %1, %0
  %i.02 = phi i32 [ 0, %0 ], [ %10, %1 ]
  %2 = getelementptr inbounds [100 x i32]* @test_data, i32 0, i32 %i.02
  %3 = load i32* %2, align 4, !tbaa !1
  %4 = or i32 %i.02, 1
  %5 = getelementptr inbounds [100 x i32]* @test_data, i32 0, i32 %4
  %6 = load i32* %5, align 4, !tbaa !1
  %7 = tail call i32 @encode(i32 %3, i32 %6) #4
  %8 = sdiv i32 %i.02, 2
  %9 = getelementptr inbounds [100 x i32]* @compressed, i32 0, i32 %8
  store i32 %7, i32* %9, align 4, !tbaa !1
  %10 = add nsw i32 %i.02, 2
  %11 = icmp slt i32 %10, 100
  br i1 %11, label %1, label %.preheader.preheader

.preheader.preheader:                             ; preds = %1
  br label %.preheader

.preheader:                                       ; preds = %.preheader, %.preheader.preheader
  %i.11 = phi i32 [ %20, %.preheader ], [ 0, %.preheader.preheader ]
  %12 = sdiv i32 %i.11, 2
  %13 = getelementptr inbounds [100 x i32]* @compressed, i32 0, i32 %12
  %14 = load i32* %13, align 4, !tbaa !1
  tail call void @decode(i32 %14) #4
  %15 = load i32* @xout1, align 4, !tbaa !1
  %16 = getelementptr inbounds [100 x i32]* @result, i32 0, i32 %i.11
  store i32 %15, i32* %16, align 4, !tbaa !1
  %17 = load i32* @xout2, align 4, !tbaa !1
  %18 = or i32 %i.11, 1
  %19 = getelementptr inbounds [100 x i32]* @result, i32 0, i32 %18
  store i32 %17, i32* %19, align 4, !tbaa !1
  %20 = add nsw i32 %i.11, 2
  %21 = icmp slt i32 %20, 100
  br i1 %21, label %.preheader, label %22

; <label>:22                                      ; preds = %.preheader
  ret void
}

; Function Attrs: nounwind
define i32 @main() #1 {
  tail call void @reset() #5
  br label %1

; <label>:1                                       ; preds = %1, %0
  %i.02.i = phi i32 [ 0, %0 ], [ %10, %1 ]
  %2 = getelementptr inbounds [100 x i32]* @test_data, i32 0, i32 %i.02.i
  %3 = load i32* %2, align 4, !tbaa !1
  %4 = or i32 %i.02.i, 1
  %5 = getelementptr inbounds [100 x i32]* @test_data, i32 0, i32 %4
  %6 = load i32* %5, align 4, !tbaa !1
  %7 = tail call i32 @encode(i32 %3, i32 %6) #5
  %8 = sdiv i32 %i.02.i, 2
  %9 = getelementptr inbounds [100 x i32]* @compressed, i32 0, i32 %8
  store i32 %7, i32* %9, align 4, !tbaa !1
  %10 = add nsw i32 %i.02.i, 2
  %11 = icmp slt i32 %10, 100
  br i1 %11, label %1, label %.preheader.i.preheader

.preheader.i.preheader:                           ; preds = %1
  br label %.preheader.i

.preheader.i:                                     ; preds = %.preheader.i, %.preheader.i.preheader
  %i.11.i = phi i32 [ %20, %.preheader.i ], [ 0, %.preheader.i.preheader ]
  %12 = sdiv i32 %i.11.i, 2
  %13 = getelementptr inbounds [100 x i32]* @compressed, i32 0, i32 %12
  %14 = load i32* %13, align 4, !tbaa !1
  tail call void @decode(i32 %14) #5
  %15 = load i32* @xout1, align 4, !tbaa !1
  %16 = getelementptr inbounds [100 x i32]* @result, i32 0, i32 %i.11.i
  store i32 %15, i32* %16, align 4, !tbaa !1
  %17 = load i32* @xout2, align 4, !tbaa !1
  %18 = or i32 %i.11.i, 1
  %19 = getelementptr inbounds [100 x i32]* @result, i32 0, i32 %18
  store i32 %17, i32* %19, align 4, !tbaa !1
  %20 = add nsw i32 %i.11.i, 2
  %21 = icmp slt i32 %20, 100
  br i1 %21, label %.preheader.i, label %adpcm_main.exit.preheader

adpcm_main.exit.preheader:                        ; preds = %.preheader.i
  br label %adpcm_main.exit

adpcm_main.exit:                                  ; preds = %adpcm_main.exit, %adpcm_main.exit.preheader
  %main_result.04 = phi i32 [ %.main_result.0, %adpcm_main.exit ], [ 0, %adpcm_main.exit.preheader ]
  %i.03 = phi i32 [ %28, %adpcm_main.exit ], [ 0, %adpcm_main.exit.preheader ]
  %22 = getelementptr inbounds [100 x i32]* @compressed, i32 0, i32 %i.03
  %23 = load i32* %22, align 4, !tbaa !1
  %24 = getelementptr inbounds [100 x i32]* @test_compressed, i32 0, i32 %i.03
  %25 = load i32* %24, align 4, !tbaa !1
  %26 = icmp eq i32 %23, %25
  %27 = zext i1 %26 to i32
  %.main_result.0 = add nsw i32 %27, %main_result.04
  %28 = add nsw i32 %i.03, 1
  %exitcond5 = icmp eq i32 %28, 50
  br i1 %exitcond5, label %.preheader.preheader, label %adpcm_main.exit

.preheader.preheader:                             ; preds = %adpcm_main.exit
  %.main_result.0.lcssa = phi i32 [ %.main_result.0, %adpcm_main.exit ]
  br label %.preheader

.preheader:                                       ; preds = %.preheader, %.preheader.preheader
  %main_result.22 = phi i32 [ %.main_result.2, %.preheader ], [ %.main_result.0.lcssa, %.preheader.preheader ]
  %i.11 = phi i32 [ %35, %.preheader ], [ 0, %.preheader.preheader ]
  %29 = getelementptr inbounds [100 x i32]* @result, i32 0, i32 %i.11
  %30 = load i32* %29, align 4, !tbaa !1
  %31 = getelementptr inbounds [100 x i32]* @test_result, i32 0, i32 %i.11
  %32 = load i32* %31, align 4, !tbaa !1
  %33 = icmp eq i32 %30, %32
  %34 = zext i1 %33 to i32
  %.main_result.2 = add nsw i32 %34, %main_result.22
  %35 = add nsw i32 %i.11, 1
  %exitcond = icmp eq i32 %35, 100
  br i1 %exitcond, label %36, label %.preheader

; <label>:36                                      ; preds = %.preheader
  %.main_result.2.lcssa = phi i32 [ %.main_result.2, %.preheader ]
  %37 = tail call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([12 x i8]* @.str, i32 0, i32 0), i32 %.main_result.2.lcssa) #5
  %38 = icmp eq i32 %.main_result.2.lcssa, 150
  br i1 %38, label %39, label %41

; <label>:39                                      ; preds = %36
  %40 = tail call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([14 x i8]* @.str1, i32 0, i32 0)) #5
  br label %43

; <label>:41                                      ; preds = %36
  %42 = tail call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([14 x i8]* @.str2, i32 0, i32 0)) #5
  br label %43

; <label>:43                                      ; preds = %41, %39
  ret i32 %.main_result.2.lcssa
}

declare i32 @printf(i8*, ...) #3

attributes #0 = { nounwind readnone "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind readonly "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #4 = { nobuiltin }
attributes #5 = { nobuiltin nounwind }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"clang version 3.5.0 (tags/RELEASE_350/final)"}
!1 = metadata !{metadata !2, metadata !2, i64 0}
!2 = metadata !{metadata !"int", metadata !3, i64 0}
!3 = metadata !{metadata !"omnipotent char", metadata !4, i64 0}
!4 = metadata !{metadata !"Simple C/C++ TBAA"}
