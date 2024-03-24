/**
 * Класс представляющий собой модель службы ремнта и обслуживания ЛВС (фармулы, расчёты,данные)
 */
package com.example.modelrepairman;

import java.math.BigDecimal;
import java.math.BigInteger;
import java.math.RoundingMode;

public class Model {
    Integer tH0, t0, N, S1, S; //начальные данные для расчётов (ввод)
    //далее поля для вывода
    Integer C;
    //значения, которые требуется расчитать (вывод)
    Double P0, Q, L, U, po, n, pe, W, Tp, Tc, pe_p0, Y;
    //для расчётов
    private int k;
    double psi;
    public static final int NOT_DECIMAL_POINT = -1;
    private static int decimal_point = NOT_DECIMAL_POINT;
    public static int getDecimal_point() {
        return decimal_point;
    }

    public static void setDecimal_point(int decimal_point) {
        Model.decimal_point = decimal_point;
    }

    public Double getP0() {
        return RoundResult(P0);
    }

    public Double getPo() {
        return RoundResult(po);
    }
    public Double getN() {
        return RoundResult(n);
    }

    public Double getPe() {
        return RoundResult(pe);
    }

    public Double getW() {
        return RoundResult(W);
    }

    public Double getTp() {
        return RoundResult(Tp);
    }

    public Double getTc() {
        return RoundResult(Tc);
    }

    public Double getPe_p0() {
        return RoundResult(pe_p0);
    }

    public Double getY() {
        return RoundResult(Y);
    }

    public Double getQ() {
        return RoundResult(Q);
    }

    public Double getL() {
        return RoundResult(L);
    }

    public Double getU() {
        return RoundResult(U);
    }

    public Integer getC() {
        return C;
    }
    /**
     * Для изменения объекта по варианту расчёта (по заданию выводить график и считать значения при изменении кол-ва ремонтников)
     * @param c
     */
    public void setC(Integer c) {
        C = c;
    }
    public Model(Integer tH0, Integer t0, Integer n, Integer c, Integer s1, Integer s) {
        this.tH0 = tH0; //среднее время наработки на отказ
        this.t0 = t0;   //среднее время обслуживания
        this.N = n;     //кол-во раб. станций
        this.C = c;     //кол-во ремонтников
        this.S1 = s1;   //ЗП ремонтника в час
        this.S = s;     //потери организации в час

        this.psi = (double)t0 / (double)tH0;
        this.P0 = calculationP0();
        this.Q = calculationQ();
        this.L = calculationL();
        this.U = calculationU();
        this.po = calculationPo();
        this.n = calculationN();
        this.pe = calculationPe();
        this.Tp = calculationTp();
        this.W = calculationW();
        this.Tc = calculationTc();
        this.pe_p0 = calculationPe_p0();
        this.Y = calculationY();
    }
    private double calculationP0(){
        BigInteger a = BigInteger.ZERO, b = BigInteger.ZERO;
        double a1 = 0, b1 = 0, a2 = 0, b2 = 0;
        int k1 = 0;
        while (k1 <= C) {
            a = (factorial(N).divide(factorial(k1).multiply(factorial(N - k1))));
            a1 = a.doubleValue();
            a1 = a1 * Math.pow(psi, k1);
            a2 = a2 + a1;
            k1++;
        }

        while (k1 <= N) {
            b = factorial(N).divide(factorial(C).multiply(factorial(N - k1)));
            b1 = b.doubleValue();
            b1 = b1 * Math.pow(psi, k1) / Math.pow(C, (k1 - C));
            b2 = b2 + b1;
            k1++;
        }

        double P0 = Math.pow((a2 + b2), -1);
        // собственно расчет вероятности состояния
        if (k == 0) {
            return P0;
        }
        if (k <= C) {
            BigInteger pkc = factorial(N).divide(factorial(k).multiply(factorial(N - k)));
            double pkc1 = pkc.doubleValue() * Math.pow(psi, k) * P0;
            return pkc1;
        }
        if (k > C) {
            BigInteger pck = factorial(N).divide(factorial(C).multiply(factorial(N - k)));
            double pck1 = pck.doubleValue() * Math.pow(psi, k) * P0 / Math.pow(C, (k - C));
            return pck1;
        }
        return -1;
    }

    private double calculationQ() {
        double q = 0.0;
        for (k = C; k <= N; k++) {
            q = q + (k - C) * calculationP0();
        }
        return q;
    }

    private double calculationL(){
        double l = 0;
        for (k = 1; k <= N; k++) {
            l = l + k * calculationP0();
        }
        return l;
    }

    private double calculationU() {
        return L - Q;
    }

    private double calculationPo() {
        return U / C;
    }

    private double calculationN() {
        return N - L;
    }

    private double calculationPe() {
        return (double)n / (double)N;
    }

    private double calculationW() {
        return Tp - t0;
    }

    private double calculationTp() {
        return (L * tH0 / (N - L));
    }

    private double calculationTc() {
        return Tp + tH0;
    }

    private double calculationPe_p0() {
        return pe / po;
    }
    private double calculationY() {
        return (C * S1 + L * S);
    }
    private BigInteger recfact(long start, long n) {
        long i;
        if (n <= 16) {
            BigInteger r = BigInteger.valueOf(start);
            for (i = start + 1; i < start + n; i++) r = r.multiply(BigInteger.valueOf(i));
            return r;
        }
        i = n / 2;
        return recfact(start, i).multiply(recfact(start + i, n - i));
    }
    private BigInteger factorial(long n) {
        return recfact(1, n);
    }

    private Double RoundResult(Double number){
        if(decimal_point != NOT_DECIMAL_POINT) {
            BigDecimal bd = new BigDecimal(number);
            BigDecimal rounded = bd.setScale(decimal_point, RoundingMode.HALF_UP);
            double result = rounded.doubleValue();
            return  result;
        }
        return number;
    }
}
