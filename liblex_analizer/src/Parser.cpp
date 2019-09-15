#include "../../../syntax/Parser.h"


using namespace std;

bool Parser::machine(){
    if(Parser::rule()){
        TokenCode token; //Inicializar el token

        if(Parser::machine() || token == EOS){
            return true;
        }
    }

    return false;
}

bool Parser::rule(){
    TokenCode token1; //inicializar el token

    if(token1 == AT_BOL){
        if(Parser::expr()){
            TokenCode token2; // inicializar token

            if(token2 == EOS){
                if(Parser::action()){
                    return true;
                }
            }

            //retroceder al lexico
        }
    }

    //retroceder al lexico



    if(Parser::expr()){
        TokenCode token2; // inicializar token

        if(token2 == EOS){
            if(Parser::action()){
                return true;
            }
        }

        if(token2 == AT_EOL){
            TokenCode token3; //init token
            if(token3 == EOS){
                if(Parser::action()){
                    return true;
                }
            }
        }
    }
    
    return false;
}

bool Parser::action(){
    if(Parser::whiteSpace()){
        // no simplificar, str mueve la posicion del lexico en la cadena
        if(Parser::str()){
            return true;
        }

        return true;
    }

    //retroceder al lexico

    return true;
}

bool Parser::expr(){

    if(Parser::catExpr()){
        if(Parser::exprp()){
            return true;
        }
    }

    return false;
}

bool Parser::exprp(){
    TokenCode token; //init token

    if(token == OR){
        if(Parser::catExpr()){
            if(Parser::exprp()){
                return true;
            }
        }
    }

    //retroceder al lexico

    return true;
}

bool Parser::catExpr(){
    if(Parser::factor()){
        if(Parser::catExprp()){
            return true;
        }
    }

    return false;
}

bool Parser::catExprp(){
    if(Parser::factor()){
        if(Parser::catExprp()){
            return true;
        }
    }
    //retroceder al analizador lexico
    return true;
}

bool Parser::factor(){
    if(Parser::term()){
        TokenCode token; //init token

        if(token == CLOSURE || token == PLUS_CLOSURE || token == OPTIONAL){
            return true;
        }

        //retroceder al analizador lexico

        return true;
    }

    return false;
}

bool Parser::term(){

    if(Parser::character()){
        return true;
    }

    TokenCode token1; //init token

    if(token1 == CCL_START){
        if(Parser::str()){
            TokenCode token2; //init token

            if(token2 == CCL_END){
                return true;
            }

            //retroceder al lexico

            return false;
        }


        TokenCode token2; //init token

        if(token2 == CCL_END){
            return true;
        }

        if(token2 == AT_BOL){
            TokenCode token3; //init token
            if(Parser::str()){
                
                if(token3 == CCL_END){
                    return true;
                }

            }

            if(token3 == CCL_END){
                return true;
            }
            //retroceder al lexico
        }

        //retroceder al lexico
        return false;
    }

    if(token1 == ANY){
        return true;
    }

    if(token1 == OPEN_PAREN){
        if(Parser::expr()){
            TokenCode token2;

            if(token2 == CLOSE_PAREN){
                return true;
            }

            //retroceder al analizador lexico
        }
    }

    return false;
}


bool Parser::character(){
    TokenCode token;

    if(token == L){
        return true;
    }

    return false;
}

bool Parser::strp(){
    if(Parser::character()){
        if(Parser::strp()){
            return true;
        }
    }

    //retroceder al analizador lexico

    return true;
}

bool Parser::str(){
    if(Parser::character()){
        if(Parser::strp())
            return true;
    }

    return false;
}

bool Parser::whiteSpace(){
    TokenCode token;

    if(token == WHITE_SPACE){
        return true;
    }

    return false;
}

