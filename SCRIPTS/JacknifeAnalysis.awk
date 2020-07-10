BEGIN{

        for(Block=0;Block<NumberOfBlocks;Block++){

            Weight[Block]=0.0;  Values[Block]=0.0;

        }

        BlockSize=int(NumberOfLines/NumberOfBlocks);
    
        CurrentBlock=0; Ns=0;

}


{
    
        EventWeight=$(wCol); Observable=$(valCol);

        Values[CurrentBlock]+=EventWeight*Observable;

        Weight[CurrentBlock]+=EventWeight;

		Ns++;

        if(Ns==BlockSize && CurrentBlock+1<NumberOfBlocks){CurrentBlock++; Ns=0;}

}


END{


        GlobalAverage=0.0; GlobalWeight=0.0;

        for(SkipBlock=0;SkipBlock<NumberOfBlocks;SkipBlock++){

                JacknifeAverage[SkipBlock]=0.0; JacknifeWeight[SkipBlock]=0.0;

                for(Block=0;Block<NumberOfBlocks;Block++){

                    if(Block!=SkipBlock){
			
                        JacknifeAverage[SkipBlock]+=Values[Block];

                        JacknifeWeight[SkipBlock]+=Weight[Block];
                    }

                }

                GlobalAverage+=JacknifeAverage[SkipBlock];
                GlobalWeight+=JacknifeWeight[SkipBlock];

            }

        GlobalResult=GlobalAverage/GlobalWeight;

        GlobalError=0.0;

        for(SkipBlock=0;SkipBlock<NumberOfBlocks;SkipBlock++){
                
        	LocalResult=JacknifeAverage[SkipBlock]/JacknifeWeight[SkipBlock];

                GlobalError+=(GlobalResult-LocalResult)*(GlobalResult-LocalResult);

        }

        GlobalError=sqrt((NumberOfBlocks-1.0)*GlobalError/NumberOfBlocks);

        print GlobalResult, GlobalError;
    

}
