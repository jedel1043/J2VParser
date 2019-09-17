#include "Parser.h"


using namespace std;

bool Parser::machine(){
    if(Parser::rule()){
        TokenCode token; //Inicializar el token

        if(Parser::machine() || token == TokenCode::EOS){
            return true;
        }
    }

    return false;
}

bool Parser::rule(){
    TokenCode token1; //inicializar el token

    if(token1 == TokenCode::AT_BOL){
        if(Parser::expr()){
            TokenCode token2; // inicializar token

            if(token2 == TokenCode::EOS){
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

        if(token2 == TokenCode::EOS){
            if(Parser::action()){
                return true;
            }
        }

        if(token2 == TokenCode::AT_EOL){
            TokenCode token3; //init token
            if(token3 == TokenCode::EOS){
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

    if(token == TokenCode::OR){
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

        if(token == TokenCode::CLOSURE || token == TokenCode::PLUS_CLOSURE || token == TokenCode::OPTIONAL){
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

    if(token1 == TokenCode::CCL_START){
        if(Parser::str()){
            TokenCode token2; //init token

            return token2 == TokenCode::CCL_END;

            //retroceder al lexico

        }


        TokenCode token2; //init token

        if(token2 == TokenCode::CCL_END){
            return true;
        }

        if(token2 == TokenCode::AT_BOL){
            TokenCode token3; //init token
            if(Parser::str()){
                
                if(token3 == TokenCode::CCL_END){
                    return true;
                }

            }

            if(token3 == TokenCode::CCL_END){
                return true;
            }
            //retroceder al lexico
        }

        //retroceder al lexico
        return false;
    }

    if(token1 == TokenCode::ANY){
        return true;
    }

    if(token1 == TokenCode::OPEN_PAREN){
        if(Parser::expr()){
            TokenCode token2;

            if(token2 == TokenCode::CLOSE_PAREN){
                return true;
            }

            //retroceder al analizador lexico
        }
    }

    return false;
}


bool Parser::character(){
    TokenCode token;

    if(token == TokenCode::L){
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

    return token == TokenCode::WHITE_SPACE;
}

