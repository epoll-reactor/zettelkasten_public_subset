// const volatile = volatile const
int main() {
                 int  _1          = {0};
  const          int  _1_c        = {0};
        volatile int  _1_v        = {0};
                 int &_0_ref_1    = _1;
//               int &_0_ref_2    = _1_c; ERROR
//               int &_0_ref_3    = _1_v; ERROR
                 int &_1_ref      =  const_cast<               int &>(_1_c);
//               int &_1_ref      = static_cast<               int &>(_1_c); ERROR
  const          int &_2_c_ref_1  =  const_cast<const          int &>(_1_c);
//const          int &_2_c_ref_2  = static_cast<const          int &>(_1_v); ERROR
//      volatile int &_3_v_ref_1  = static_cast<      volatile int &>(_1_c); ERROR
        volatile int &_3_v_ref_2  =  const_cast<      volatile int &>(_1_c);
  const volatile int &_4_cv_ref_1 = static_cast<const volatile int &>(_1_c);
  const volatile int &_4_cv_ref_2 =  const_cast<const volatile int &>(_1_c);
  const volatile int &_4_cv_ref_3 = static_cast<const volatile int &>(_1_v);
  const volatile int &_4_cv_ref_4 =  const_cast<const volatile int &>(_1_v);
}
