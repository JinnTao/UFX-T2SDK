#ifndef DATA_DEF_H_
#define DATA_DEF_H_

//33100 ��������(�ڲ��ṹ��->����ģ�����)
struct CMarketInfo_UFX
{
	char     quote_type;                  //��������(0-������ͨ����1-��������)                 
	int      quote_length;                //�������ݳ��ȣ��ӵ�3�ֶο�ʼ�����һ���ֶμ��㳤�ȣ�
	int      update_time;                 //ʱ���(eg:144510500,14��45��10��500����)           
	int      quote_index;                 //�������                                           
	char     futu_exch_type[5];           //�������                                           
	char     commodity_type[7];           //��Ʒ����                                           
	char     contract_code[31];           //��Լ����                                           
	double   pre_settlement_price;        //���ս����                                         
	double   pre_close_price;             //�������̼�                                         
	double   pre_open_interest;           //���տ�����                                         
	double   pre_delta;                   //������ʵ��                                         
	double   futu_open_price;             //���̼�                                             
	double   futu_last_price;             //���¼۸�                                           
	double   buy_high_price;              //��������                                         
	double   buy_high_amount;             //������������                                     
	double   sale_low_price;              //������ۼ۸�                                       
	double   sale_low_amount;             //�����������                                       
	double   futu_high_price;             //��߼�                                             
	double   futu_low_price;              //��ͼ�                                             
	double   uplimited_price;             //��ͣ��                                             
	double   downlimited_price;           //��ͣ��                                             
	double   average_price;               //����                                               
	double   change_direction;            //����                                               
	double   business_amount;             //�ɽ���                                             
	double   open_interest;               //�ܳ���                                             
	double   business_balance;            //�ɽ���                                             
	double   futu_close_price;            //�������̼�                                         
	double   settlement_price;            //�����                                             
	int      actionday;                   //��ǰ����(yyyymmdd)                                 
};

//33101 ί�лر�
struct COrderRspInfo_UFX
{
	char     entrust_no[10];               //ί�к�                                                
	char     futures_account[13];          //���ױ���                                 
	char     futu_exch_type[4];            //���������F1-֣��F2����-F3�Ϻ�-F4�н�
	char     contract_code[31];            //��Լ����                                 
	char     entrust_bs;                   //������ʶ(1-���� 2-����)                  
	char     entrust_direction;            //��ƽ��ʶ(1-����2-ƽ��4-ƽ��֣�����Ϊ��) 
	char     hedge_type;                   //�ױ���ʶ(0-Ͷ��1-�ױ�2-����)             
	char     fund_account[19];             //�ʽ��˻�                                 
	char     futu_report_no[22];           //���ص���                                 
	char     firm_no[22];                  //��Ա��                                   
	char     operator_no[19];              //����Ա��                                 
	int      client_group;                 //�ͻ����                                 
	double   entrust_amount;               //ί������                                 
	double   business_total_amount;        //�ɽ�������                               
	double   cacel_amount;                 //��������                                 
	double   entrust_price;                //ί�м۸�                                 
	char     entrust_status;               //ί��״̬                                 
	int      branch_no;                    //Ӫҵ����                                 
	int      batch_no;                     //ί������                                 
	char     futu_entrust_type;            //ί������                                 
	int      amount_per_hand;              //��Լ����                                 
	char     forceclose_reason;            //ǿƽԭ��                                 
	char     init_date[9];                 //��������                                 
	char     curr_time[9];                 //��ǰʱ��                                 
	char     confirm_no[21];               //��������                                 
	char     entrust_occasion[33];         //ί�г���                                 
	double   entrust_price2;               //ί������(ί������۸�)     
	char     futu_entrust_prop[4];         //�ڻ�ί������
	char     arbitrage_code[31];           //���ί��
	char     entrust_reference[33];        //ί������
	char     error_message[33];            //����˵��
	char     position_str[33];             //��λ��
	//int      request_id;                   //������
	//int      session_id;                   //�Ự���
};

//33101 �ɽ��ر�
struct CRealRspInfo_UFX
{
	char     entrust_no[10];               //ί�к�                                  
	char     futures_account[13];          //���ױ���                                
	char     futu_exch_type[5];            //���������                              
	char     business_no[10];              //�ɽ����                                
	char     contract_code[31];            //��Լ����                                
	char     entrust_bs;                   //������ʶ(1-���� 2-����)                 
	char     entrust_direction;            //��ƽ��ʶ(1-����2-ƽ��3-ƽ���  ����Ϊ��)
	double   business_price;               //�ɽ��۸�                                
	double   business_amount;              //�ɽ�����                                
	char     hedge_type;                   //�ױ���ʶ(0-Ͷ��1-�ױ�2-����)            
	char     fund_account[19];             //�ʽ��˻�                                
	char     futu_report_no[22];           //���ص���                                
	char     firm_no[22];                  //��Ա��                                  
	char     operator_no[18];              //����Ա��                                
	int      client_group;                 //�ͻ����                                
	double   entrust_amount;               //ί������                                
	double   business_total_amount;        //�ɽ�������                              
	double   cacel_amount;                 //��������                                
	double   entrust_price;                //ί�м۸�                                
	char     entrust_status;               //ί��״̬                                
	int      branch_no;                    //Ӫҵ����                                
	int      batch_no;                     //ί������                                
	char     futu_entrust_type;            //ί������                                
	int      amount_per_hand;              //��Լ����                                
	char     forceclose_reason;            //ǿƽԭ��                                
	char     init_date[9];                 //��������                                
	char     business_time[9];             //�ɽ�ʱ��                                
	char     confirm_no[21];               //��������                                
	char     entrust_occasion[33];         //ί�г���                                
	double   entrust_price2;               //ί������(ί������۸�)                   
	char     futu_entrust_prop[4];         //�ڻ�ί������
	char     arbitrage_code[31];           //���ί��
	char     entrust_reference[33];        //ί������
	char     position_str[33];             //��λ��
	//int      request_id;                   //������(��ʱ)
	//int      session_id;                   //�Ự���(��ʱ)
};

//33102 �������(�ڲ��ṹ��->����ģ�����)
struct CArgMarketInfo_UFX
{
	char    arbicontract_id[31];        //������Լ��           
	char    futu_exch_type[5];          //�������             
	char    first_code[31];             //��һ��               
	char    second_code[31];            //�ڶ���               
	char    weave_type;                 //�������1-SPD, 2 -IPS
	double  buy_price;                  //��������           
	double  buy_amount;                 //������������       
	double  buy_total_amount;           //ȫ������             
	double  sale_price;                 //������ۼ۸�         
	double  sale_amount;                //�����������         
	double  sale_total_amount;          //ȫ������             
	double  futu_high_price;            //��߼�               
	double  futu_low_price;             //��ͼ�               
	double  uplimited_price;            //��ͣ��۸�           
	double  downlimited_price;          //��ͣ��۸�  
};

//33105 ������״̬֪ͨ
struct CExchangeStatus_UFX
{
	char      futu_exch_type[5];       //���������F1-֣��F2����-F3�Ϻ�-F4��?
	char      exch_status;             //������״̬��0���ӶϿ�,1����ǰ, 2���ϱ�
	char      entrade_flag;            //�Ƿ�ɽ��ף�0���ɽ��ף�1�ɽ��ף�      
};

#endif //DATA_DEF_H_

