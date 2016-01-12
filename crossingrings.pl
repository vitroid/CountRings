#!/usr/bin/env perl
# -*- coding: utf-8 -*-
#
#Looking up the crossing rings.
#
use strict;

my $debug = ( $ARGV[0] eq "-d" );

while(<STDIN>){
  if ( /^\@RNGS/ ){
    if ( ! $debug ){
      print;
    }
    my $nnode = <STDIN>;
    if ( ! $debug ){
      print $nnode;
    }
    my @ringsatthenode;
    my @nodesatthering;
    my $nring=0;
    while(<STDIN>){
      chomp;
      my ( @nodes ) = split;
      last if ( shift @nodes ) <= 0;
      $nodesatthering[$nring] = [ @nodes ];
      foreach my $node ( @nodes ){
	push @{$ringsatthenode[$node]}, $nring;
      }
      $nring ++;
    }
    #
    #隣接する環が共有する節点の表を作る。
    #
    my @ringring;
    foreach my $node ( 0..$#ringsatthenode ){
      if ( $ringsatthenode[$node] ){
	my @rings = @{$ringsatthenode[$node]};
	for(my $i=0; $i<=$#rings; $i++ ){
	  my $ii = $rings[$i];
	  for(my $j=$i+1; $j<=$#rings; $j++ ){
	    my $jj = $rings[$j];
	    push @{$ringring[$ii][$jj]}, $node;
	    push @{$ringring[$jj][$ii]}, $node;
	  }
	}
      }
    }
    my @omit;
    foreach my $i ( 0..$#ringring ){
      if ( ! $omit[$i] ){
	my @nodes = @{$nodesatthering[$i]};
	foreach my $j ( 0.. $#{$ringring[$i]} ){
	  if ( $i < $j ){
	    if ( ! $omit[$j] && defined $ringring[$i][$j]){
	      my @shared = @{$ringring[$i][$j]};
	      next if $#shared<0;
	      #print join( " ", $i, $j, @shared ), "\n";
	      #print join(":", @nodes), " nodes\n";
	      my @mark;
	      foreach my $s ( @shared ){
		$mark[$s] ++;
	      }
	      my $block=0;
	      my $isone=(0<$mark[$nodes[$#nodes]]);
	      foreach my $node ( @nodes ){
		#立ち上がり
		if ( ! $isone && $mark[$node] ){
		  $block++;
		  $isone = 1;
		}
		#立ち下がり
		if ( $isone && ! $mark[$node] ){
		  $isone = 0;
		}
	      }
	      #立ち上がりが2回以上あれば、交差がおきている。
	      if ( 1 < $block ){
		if ( $debug ){
		  print "$i $j\n";
		  print join(":", @nodes), " nodes\n";
		  print join(":", @{$nodesatthering[$j]}), " nodes\n";
		  print join(":", @{$ringring[$i][$j]}), " shared\n";
		}
		else{
		  print STDERR "[" . ($#{$nodesatthering[$j]}+1) . "]";
		}
		$omit[$j] = 1;
	      }
	    }
	  }
	}
	if ( ! $debug ){
	  print join( " ", $#nodes+1, @nodes ), "\n";
	}
      }
    }
    if ( ! $debug ){
      print "0\n";
    }
  }
}
